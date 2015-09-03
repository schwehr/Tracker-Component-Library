function [xUpdate, SUpdate,innov,Szz]=sqrtCubStackedKalUpdate(xPred,SPred,z,SR,h,xi,w)
%%SQRTCUBSTACKEDKALUPDATE Perform the measurement update step in the square  
%                         root cubature Kalman filter with non-additive
%                         measurement noise.
%
%INPUTS:    xPred           The xDim X 1 predicted target state.
%           SPred           The xDim X xDim lower-triangular predicted
%                           state covariance matrix.
%           z               The zDim X 1 vector measurement.
%           SR              The zDim X zDim lower-triangular square root of  
%                           the measurement covariance matrix in the native 
%                           coordiante system of the measurement.
%           h               A function handle for the measurement function
%                           that takes the stacked state and measurement
%                           noise as its argument.
%           xi              An (xDim+zDim) X numCubPoints matrix of 
%                           cubature points for the stacked state and
%                           measurement noise vector.
%           w               A numCubPoints X 1 vector of the weights 
%                           associated with the cubature points.
%
%OUTPUTS    xUpdate     The xDim X 1 updated state vector.
%           SUpdate     The updated xDim X xDim lower-triangular square 
%                       root state covariance matrix.
%       innov, Szz      The zDimX1 innovation and the zDimXzDim square
%                       root innovation covariance matrix are returned
%                       in case one wishes to analyze the consistency
%                       of the estimator or use those values in gating or
%                       likelihood evaluation.
%
%%The mathematics behind the function sqrtCubStackedKalUpdate are described
%in more detail in Section IX of
%David F. Crouse , "Basic tracking using nonlinear 3D monostatic and
%bistatic measurements," IEEE Aerospace and Electronic Systems Magazine,
%vol. 29, no. 8, Part II, pp. 4-53, Aug. 2014.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    zDim=size(z,1);
    numCubPoints=size(xi,2);
    sqrtW=sqrt(w);

    %Predicted cubature state points
    S=[SPred,               zeros(xDim,noiseDim);
       zeros(noiseDim,xDim),SR];
    xPredStackedPoints=transformCubPoints(xi,[xPred;zeros(zDim,1)],S);

    %Predicted, centered cubature state points
    xPredCenPoints=bsxfun(@times,bsxfun(@minus,xPredStackedPoints(1:xDim,:),xPred),sqrtW');

    %Predicted cubature measurement points
    zPredPoints=zeros(zDim,numCubPoints);
    for curP=1:numCubPoints
        zPredPoints(:,curP)=h(xPredStackedPoints(:,curP));
    end
    
    %Measurement prediction
    zPred=calcMixtureMoments(zPredPoints,w);
    
    %The innovation
    innov=z-zPred;

    %Centered, predicted cubature measurement points
    zPredCenPoints=bsxfun(@times,bsxfun(@minus,zPredPoints,zPred),sqrtW');

    %Root innovation covariance
    Szz=tria(zPredCenPoints);

    %The cross covariance.
    Pxz=xPredCenPoints*zPredCenPoints';

    %The filter gain
    W=(Pxz/Szz')/Szz;

    %Updated state estimate
    xUpdate=xPred+W*innov;

    %Updated state root covariance
    SUpdate=tria(xPredCenPoints-W*zPredCenPoints);
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
