function [delay,uArrive,uDepart]=atmosRefracMeas(xObs,xObj,NObs,NObsLoc)
%%ATMOSREFRACMEAS  Given the location of an observer and an object in the
%                  atmosphere of the Earth, find the delay and angle of
%                  arrival of a signal from the object to the observer,
%                  accounting for basic,standard  refraction. A
%                  low-fidelity exponential atmospheric model is used. This
%                  function is not suitable for computing refraction
%                  between satellites, grazing the Earth's atmosphere. The
%                  algorithm might have an error if the raypath
%                  goes too far underground.
%
%INPUTS: xObs The Cartesian location of the observer in ECEF coordinates in
%             meters as [x;y;z].
%        xObj The Cartesian location of the object being observed in
%             ECEF coordinates in meters as [x;y;z].
%        NObs The refractivity as measured at the observer or at sea level.
%             If this parameter is omitted, then a value of refractivity at
%             sea level of 277 will be used.
%     NObsLoc The location of the measurement of NObs. Possible values are
%             0 (The default if this parameter is omitted) sea level.
%             1 The refractivity measurement is collocated with the
%               observer.
%
%OUTPUTS: delay  The delay (in seconds) of a signal sent from the object to
%                the observer.
%        uArrive A unit vector in ECEF coordinates pointing in the apparent
%                direction of the signal the observer received (as seen by
%                the observer).
%        uDepart The direction of the signal departing the object that
%                arrives at the observer. Put another way, if the observer
%                were to transmit a signal to the object, this is the
%                apparent direction of the observer as seen by the object.
%
%This function implements the refraction algorithm for the basic
%exponential atmosphere as described in
%"Basic Tracking Using 3D Monostatic and Bistatic Measurements in
%Refractive Environments" by David F. Crouse
%If the points are collocated, then NaNs are returned for uArrive and
%uDepart.
%
%The function will fail for paths that go too deep into the Earth.
%
%277 is the default refractivity at sea level if the user does not provide
%anything. If the user provides an unrealistic refractivity at the observer
%then the conversion in this function to a value at sea level might fail.
%The refractivity at sea level is probably not over 450, as mentioned in 
%B. R. Bean, B. A. Cahoon, C. A. Samson, and G. D. Thayer, A World Atlas of
%Atmospheric Radio Refractivity. Washington, D.C.: U. S. Department of
%Commerce, National Bureau of Standards, 1966.
%
%May 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

c=Constants.speedOfLight;

%The location of the observer in ellipsoidal coordinates.
plhPoint=Cart2Ellipse(xObs);

%Find the radius of the Earth at the location of the observer. Use the
%WGS-84 ellipsoidal Earth approximation. This will be the radius used in
%the local spherical Earth approximation for computing atmospheric
%refraction.
r0=norm(proj2Ellips(xObs));

if(nargin<3)
%An assumed default refractivity at sea level if none is provided.
    Ns=277;
else
    if(nargin<4||NObsLoc==1)
        Ns=NsObs;
    else
    %Get the refractivity at sea level using the exponential atmospheric
    %model by iterating Equation 16 from Crouse's paper and hoping that it
    %is a contraction mapping. If NObs is too large, it is possible that
    %there is no usable, stable solution and this might just converge to
    %zero or to something large.
        Ns=NObs;
        NPrev=Inf;
        while(abs(Ns-NPrev)>100*eps(Ns))
            NPrev=Ns;
            Ns=NObs*(Ns/(Ns-7.32*exp(0.005577*Ns)))^(plhPoint(3)/1000);
        end
    end
end

if(Ns==0)
    error('The conversion of NObs to sea level resulted in a zero refractivity. NObs might be unsuitable for this model.') 
end

if(Ns>450)
    warning('The conversion of NObs to sea level resulted in a refractivity above 450. This might be unrealistically large.') 
end

%We need the conversion from the 3D coordinate system of the observer and
%object into the 2D coordiante system used for raytracing. The 2D
%coordinate system has the center of the Earth as its origin and the x-y
%axes are in the plane of the vector from the observer to the target. One
%vector common to both coordinate systems in the local up vector, which
%will be the local y axis. The second vector common to both will be the
%local x vector, which will be the projection of xObj-xObs onto the local
%tangent plane. Here, the vertical is the WGS-84 vertical, since the
%precision of the model is low enough that the difference between the
%WGS-84 vertical and the gravitational vertical should not matter.
uENU=getENUAxes(plhPoint);
uVertGlobal=uENU(:,3);
uVertLocal=[0;1;0];

vec2TarGlobal=xObj-xObs;

%The projection of the xObj-xObs vector into the local tangent plane can be
%obtained by subtracting the component of the vector that is orthogonal to
%the plane.
uHorizGlobal=vec2TarGlobal-dot(vec2TarGlobal,uENU(:,3))*uENU(:,3);

%If the two points are nearly vertical, then the ray tracing algorithm will
%fail. For nearly vertical points, the refraction in the model should be
%negligible, so the simple refraction-free solution will be used.
if(norm(uHorizGlobal)<1e-3)
    delay=(1/c)*norm(vec2TarGlobal);
    uArrive=vec2TarGlobal/norm(vec2TarGlobal);
    uDepart=-uArrive;
    return;
end

uHorizGlobal=uHorizGlobal/norm(uHorizGlobal);
uHorizLocal=[1;0;0];

%Find the rotation matrix from the global coordinate system into the local
%coordinate system.
ECEF2LocalRot=findTransParam([uVertLocal,uHorizLocal],[uVertGlobal,uHorizGlobal]);

%The third (z) coordinate in the local system should be zero after this
%transformation.
vec2TarLocal=ECEF2LocalRot*vec2TarGlobal;

%The location of the receiver in the local 2D coordinate system.
x0Init=0;
y0Init=norm(xObs);

%The location of the target in the local 2D coordinate system.
x1Init=vec2TarLocal(1);
y1Init=vec2TarLocal(2)+y0Init;

%Now, set up the boundary-value problem to determine the path taken by
%light between the target and the receiver.

%The initial guess is just the linear solution. The solver requires a fixed
%number of steps. 20 is probably sufficient for things near the Earth. that
%is, up to distances of, say 400km. We can scale the number of steps as 20
%for every 400 kilometers with a minum of, say 10.
%Things outside of the atmosphere should use the astronomical refraction
%routines.
numSteps=max(10,ceil(20*norm(vec2TarLocal)/400e3));
x=linspace(x0Init,x1Init,numSteps);
slope=(y1Init-y0Init)/(x1Init-x0Init);
b=y1Init-slope*x1Init;%The y-intercept.
%The initial estimate of the solution.
solInit=bvpinit(x,@(x)[x*slope+b;slope]);

%Now, solve the differential equation.
oldOpts=bvpset();
newOpts=bvpset(oldOpts,'RelTol',1e-8,'AbsTol',1e-8,'FJacobian',@(x,y)odefunJacob(x,y,Ns,r0),'BCJacobian',@bcfunJacob);%Increase the accuracy.
sol=bvp5c(@(x,y)expDiffEq(x,y,Ns,r0),@(y0,y1)bcfun(y0,y1,y0Init,y1Init),solInit,newOpts);

%Get the delay for a signal traveling from the object to the observer. 
delay=(1/c)*integral(@(x)pathFun2D(x,sol,Ns,r0),x0Init,x1Init);

%Get the angle of arrival for a signal traveling from the object to the
%observer. The angle is determined by the slope at the initial point.
thetaOrig=atan(sol.y(2,1));
uLocal=[cos(thetaOrig);sin(thetaOrig);0];

%The inverse rotation is given the the transpose of the rotation matrix.
%This is the apparent direction of the object as seen by the observer.
uArrive=ECEF2LocalRot'*uLocal;

thetaEnd=atan(sol.y(2,end));
uLocal=[cos(thetaEnd);sin(thetaEnd);0];
%This is the apparent direction of the observer as seen by the object.
uDepart=-ECEF2LocalRot'*uLocal;
end

function val=pathFun2D(x,sol,Ns,r0)
    %This function is used to integrate the time taken
    y=deval(x,sol);
    val=(1+NRefracExp(x,y(1,:),Ns,r0)).*sqrt(1+y(2,:).^2);
end

function res=bcfun(y0,y1,y0Init,y1Init)
    %The residue to define the boundary condition for the numeric
    %differential equation solver as applied to the 2D exponential
    %atmospheric model.

    res=[y0(1)-y0Init;
         y1(1)-y1Init];
end

function [dbcy0,dbcy1]=bcfunJacob(~,~)
    %The Jacobians of the boundary conditions for raytracing the 2D
    %exponential atmospheric refraction model.
    dbcy0=[1 0
           0 0];
    dbcy1=[0 0
           1 0];
end

function J=odefunJacob(x,y,Ns,r0)
    %The Jacobian of the differential equation for raytracing the 2D
    %exponential atmospheric model.
    [expVal,ce]=NRefracExp(x,y(1),Ns,r0);

    J=zeros(2,2);
    J(1,2)=1;
    J(2,1)=ce*(1+y(2)^2)*(-expVal)*(ce*y(1)*(x*y(2)-y(1))*sqrt(x^2+y(1)^2)+x*(x+y(1)*y(2))*(expVal+1))/((x^2+y(1)^2)^(3/2)*(expVal+1)^2);
    J(2,2)=ce*(x-2*y(1)*y(2)+3*x*y(2)^2)*expVal/((expVal+1)*sqrt(x^2+y(1)^2));
end

function dxdy=expDiffEq(x,y,Ns,r0)
    %Find the refractivity at location (x,y).
    [expVal,ce]=NRefracExp(x,y(1),Ns,r0);

    dxdy=[y(2)
          ce*(1+y(2)^2)*(x*y(2)-y(1))*expVal/((expVal+1)*sqrt(x^2+y(1)^2))];
end

function [nRefrac,ce]=NRefracExp(x,y,Ns,r0)
    %The change in refractivity at an elevation of 1km based on the
    %refractivity on the surface of the Earth.
    DeltaN=-7.32*exp(0.005577*Ns);
    ce=log(Ns/(Ns+DeltaN))/1000;%Units of inverse meters.

    %The refractivity. This is the index of refraction-1
    nRefrac=1e-6*Ns*exp(-ce*(sqrt(x.^2+y.^2)-r0));
end

%LICENSE:
%
%The source code is in the public domain and not licensed or under
%copyright. The information and software may be used freely by the public.
%As required by 17 U.S.C. 403, third parties producing copyrighted works
%consisting predominantly of the material produced by U.S. government
%agencies must provide notice with such work(s) identifying the U.S.
%Government material incorporated and stating that such material is not
%subject to copyright protection.
%
%Derived works shall not identify themselves in a manner that implies an
%endorsement by or an affiliation with the Naval Research Laboratory.
%
%RECIPIENT BEARS ALL RISK RELATING TO QUALITY AND PERFORMANCE OF THE
%SOFTWARE AND ANY RELATED MATERIALS, AND AGREES TO INDEMNIFY THE NAVAL
%RESEARCH LABORATORY FOR ALL THIRD-PARTY CLAIMS RESULTING FROM THE ACTIONS
%OF RECIPIENT IN THE USE OF THE SOFTWARE.
