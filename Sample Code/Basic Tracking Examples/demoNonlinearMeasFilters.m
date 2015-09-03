function demoNonlinearMeasFilters()
%%DEMONONLINEARMEASFILTERS This demonstrates the use of basic target
%               tracking filters on a simple scenario involving nonlinear
%               measurements and a linear dynamic model. The instance of a
%               moving observer tracking an object using in 2D angle-only
%               measurements is considered. The object is observable if the
%               observer is more maneuverable than the object.
%
%The problem of tracking using 2D angular measurements is given as an
%example in Chapter 10.3.4 of
%Y. Bar-Shalom, X. R. Li, and T. Kiruabarajan, Estimation with Applications
%to Tracking and Navigation. New York: Wiley Interscience,
%2001.
%
%The example here is closer to that used in
%O. Straka, J. Duník, and M. Simandl, "Design of pure propagation
%unscented Kalman filter," in Proceedings of the 19th World Congress of
%the The International Federation of Automatic Control, Cape Town,
%South Africa, 24-29 Aug. 2014, pp. 5399-5938.
%A platform moves around, in this instances making instaneous direction
%changes, and takes angle-only measurements of a sensor. To demonstrate the
%algorithms, no actual track initiation routine is used. Rather, a random
%initialization (given an accurate prior) is used. 
%
%The use of angular measurements also demonstrates the need for the
%innovation and measurement averaging transformations as described in 
%D. F. Crouse, "Cubature/ unscented/ sigma point Kalman filtering with
%angular measurement models," in Proceedings of the 18th International
%Conference on Information Fusion, Washington, D.C., 6-9 Jul. 2015.
%which apply to the pure propagation filter and the square root cubature
%Kalman filter. The same transformation also applies to variants of the
%EKF, though it might take a few Monte Carlo runs to see how very large
%errors can arise when not properly wrapping innovation values.
%
%Running this displays the tracks from five filters for the scenario. In
%this instance, all of the algorithms have comparable accuracy.
%
%August 2015 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

T=60;%60 second sampling interval

F=[1, 0, T, 0;
   0, 1, 0, T;
   0, 0, 1, 0;
   0, 0, 0, 1];%State transition matrix,
Gamma=[0.5*T^2, 0;
         0,     0.5*T^2;
         T,     0;
         0,     T];
%Singular process noise covariance with units of m^2/s^4;
sigma=sqrt(1.6e-6);
QReduced=sigma^2*eye(2,2);
SQReduced=chol(QReduced,'lower');
Q=Gamma*QReduced*Gamma';
SQ=cholSemiDef(Q,'lower');

%Measurement noise covariance.
R=(1*(pi/180))^2;%1 degree standard deviation (converted to radians)/

%Create the true course of the target and of the platform.
numSteps=50;

%Initial target heading -140 degrees (clockwise from y axis).
theta=-140*(pi/180);
v=convertSpeedUnits(4,'nml','h','m','s');%knots to m/2
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xTargetInit=[6e3;0*2e3;xVel;yVel];

%The target just goes in a straight line.
xTarget=zeros(4,numSteps);
xTarget(:,1)=xTargetInit;
for k=2:numSteps
    xTarget(:,k)=F*xTarget(:,k-1)+SQ*randn(4,1);
end

%Initial heading +140 degrees (clockwise from y axis).
theta=140*(pi/180);
v=convertSpeedUnits(5,'nml','h','m','s');%knots to m/2
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xPlatformInit=[0;0;xVel;yVel;0.002];

xPlatform=zeros(5,numSteps);
xPlatform(:,1)=xPlatformInit;
for k=2:numSteps
    xPlatform(:,k)=FPolarCoordTurn2D(T,xPlatform(:,k-1))*xPlatform(:,k-1);
end

%Create the trajectory of the platform. It makes instantaneous turns.
xPlatform=zeros(4,numSteps);
xPlatform(:,1)=xPlatformInit(1:4);
for k=2:13
    xPlatform(:,k)=F*xPlatform(:,k-1);
end

theta=18*(pi/180);
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xPlatform(3:4,13)=[xVel;yVel];
for k=14:24
    xPlatform(:,k)=F*xPlatform(:,k-1);
end

theta=90*(pi/180);
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xPlatform(3:4,24)=[xVel;yVel];
for k=25:30
    xPlatform(:,k)=F*xPlatform(:,k-1);
end

theta=180*(pi/180);
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xPlatform(3:4,30)=[xVel;yVel];
for k=31:35
    xPlatform(:,k)=F*xPlatform(:,k-1);
end

theta=-18*(pi/180);
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xPlatform(3:4,35)=[xVel;yVel];
for k=36:40
    xPlatform(:,k)=F*xPlatform(:,k-1);
end

theta=-60*(pi/180);
xVel=v*cos(theta-pi/2);
yVel=-v*sin(theta-pi/2);
xPlatform(3:4,40)=[xVel;yVel];
for k=41:numSteps
    xPlatform(:,k)=F*xPlatform(:,k-1);
end

%Create the measurements
z=zeros(1,numSteps);
for k=1:numSteps
    z(:,k)=measFunc(xTarget(:,k),xPlatform(:,k))+sqrt(R)*randn(1,1);
end

%Just use a simple random initialization.
PInit=[1e5,    0,     0,   0;
       0,   1e5   0,   0;
       0,       0,         2    0;
       0,       0,         0    2];
SInit=chol(PInit,'lower');
xInit=xTarget(:,1)+SInit*randn(4,1);%Initial predicted value.

%The initial point distribution will be fifth-order cubature points.
[xi,w]=fifthOrderCubPoints(4);
xiPP=bsxfun(@plus,SInit*xi,xInit);

f=@(x)(F*x);%The dynamic model.
%The wrapping function for the innovation.
innovTrans=@(innov)wrapRange(innov,-pi,pi);
%The function for taking an average of the angular measurements.
measAvgFun=@(z,w)meanAng(z,w');

%The pure propagation filter
xEstPP=zeros(4,numSteps);
xEstPP(:,1)=xInit;

%The square root CKF
xEstCKF=zeros(4,numSteps);
xEstCKF(:,1)=xInit;
SCKF=chol(PInit,'lower');
SR=chol(R,'lower');

%The extended Kalman filter
xEstEKF=zeros(4,numSteps);
xEstEKF(:,1)=xInit;
PEKF=PInit;

%The square root extended Kalman filter
xEstSEKF=zeros(4,numSteps);
xEstSEKF(:,1)=xInit;
SEKF=chol(PInit,'lower');

%The extended square root information filter
xEstESRIF=zeros(4,numSteps);
PInvSqrtESRIF=inv(chol(PInit,'lower'));
yEstESRIF=PInvSqrtESRIF*xInit;
xEstESRIF(:,1)=xInit;

for curStep=2:numSteps
    %The measurement function for all three filters.
    h=@(xState)(measFunc(xState,xPlatform(:,curStep)));
    %The Jacobian for the EKF.
    HJacob=@(xState)(measFuncJacob(xState,xPlatform(:,curStep)));
    
    %%Predict and update the pure propagation filter.
    xiPP=purePropDiscPred(xiPP,w,f,Q);
    [xiPP,xEstPP(:,curStep)]=purePropUpdate(xiPP,w,z(curStep),R,h,innovTrans,measAvgFun);
    
    %%Predict and update the square root Cubature Kalman filter.
    [xPred, SCKF]=sqrtDiscKalPred(xEstCKF(:,curStep-1),SCKF,F,SQ);
    [xEstCKF(:,curStep), SCKF]=sqrtCubKalUpdate(xPred,SCKF,z(curStep),SR,h,xi,w,innovTrans,measAvgFun);
    
    %Predict and update the EKF
    [xPred, PEKF]=DiscKalPred(xEstEKF(:,curStep-1),PEKF,F,Q);
    [xEstEKF(:,curStep),PEKF]=EKFUpdate(xPred,PEKF,z(curStep),R,h,HJacob,[],[],innovTrans);
    
    %Predict and update the square root EKF
    [xPred, SEKF]=sqrtDiscKalPred(xEstSEKF(:,curStep-1),SEKF,F,SQ);
    [xEstSEKF(:,curStep),SEKF]=sqrtEKFUpdate(xPred,SEKF,z(curStep),SR,h,HJacob,innovTrans);
    
    %Predict and update the extended square root information filter.
    [ySqrtPred, PInvSqrtESRIF]=sqrtInfoFilterDiscPred(yEstESRIF,PInvSqrtESRIF,F,SQReduced,[],Gamma);
    [yEstESRIF,PInvSqrtESRIF]=ESRIFUpdate(ySqrtPred,PInvSqrtESRIF,z(curStep),SR,h,HJacob,innovTrans);
    %Get the state estimate from the square root ifnormation estimate.
    xEstESRIF(:,curStep)=PInvSqrtESRIF\yEstESRIF;
end

figure(1)
clf
hold on
plot(xPlatform(1,:)/1e3,xPlatform(2,:)/1e3,'-r','linewidth',2)%The sensor
plot(xTarget(1,:)/1e3,xTarget(2,:)/1e3,'--b','linewidth',2)%The target
plot(xEstPP(1,:)/1e3,xEstPP(2,:)/1e3,':g','linewidth',4)%The estimate from pure propagation
plot(xEstCKF(1,:)/1e3,xEstCKF(2,:)/1e3,'-c','linewidth',2)%The estimate from the CKF
plot(xEstEKF(1,:)/1e3,xEstEKF(2,:)/1e3,'-m','linewidth',2)%The estimate from the EKF
plot(xEstSEKF(1,:)/1e3,xEstSEKF(2,:)/1e3,'-y','linewidth',2)%The estimate from the square root EKF
plot(xEstESRIF(1,:)/1e3,xEstESRIF(2,:)/1e3,'--k','linewidth',2)%The estimate from the extended square root information filter

h1=xlabel('x (km)');
h2=ylabel('y (km)');
set(gca,'FontSize',14,'FontWeight','bold','FontName','Times')
set(h1,'FontSize',14,'FontWeight','bold','FontName','Times')
set(h2,'FontSize',14,'FontWeight','bold','FontName','Times')

legend('Sensor Trajectory','Target Trajectory','Pure Propagation Filter', 'Square Root CKF','EKF','Square Root EKF', 'ESRIF','Location','SouthEast') 

end

function z=measFunc(xTarget,xPlatform)
    x=xTarget(1);
    y=xTarget(2);
    xp=xPlatform(1);
    yp=xPlatform(2);

    z=atan2((x-xp),(y-yp));
end

function J=measFuncJacob(xTarget,xPlatform)
    x=xTarget(1);
    y=xTarget(2);
    xp=xPlatform(1);
    yp=xPlatform(2);

    J=zeros(1,4);
    J(1)=(y-yp)/((x-xp)^2+(y-yp)^2);
    J(2)=(-x+xp)/((x-xp)^2+(y-yp)^2);
    
    z=atan2((x-xp),(y-yp));
end

