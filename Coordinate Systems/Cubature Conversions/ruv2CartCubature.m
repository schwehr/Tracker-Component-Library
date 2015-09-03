function [zCart, RCart] = ruv2CartCubature(z,SR,useHalfRange,zTx,zRx,M,xi,w)
%RUV2CARTCUBATURE Use cubature integration to approximate the moments of
%                 the measurement converted from bistatic r-u-v coordinates
%                 into Cartesian coordinates. For a two-way monostatic
%                 conversion, set zTx=[0;0;0]; to make the transmitter and
%                 receiver collocated.
%
%INPUTS:    z   A 3 X 1 vector with elements [r;u;v], where r is the
%               bistatic range from the transmitter to the target
%               to the receiver, and u and v are direction cosines.
%           SR  The 3 X 3 lower-triangular square root of the measurement
%               covariance matrix.
% useHalfRange  A boolean value specifying whether the bistatic range value
%               has been divided by two. This normally comes up when
%               operating in monostatic mode, so that the range reported is
%               a one-way range. The default if this parameter is not
%               provided is false.
%           zTx The 3 X 1 [x;y;z] location vector of the transmitter in the
%               local array face coordiante system of the receiver.
%           zRx The 3X1 [x;y;z] location vector of the receiver in
%               Cartesian coordinates. If this parameter is omitted or is
%               zero, thenthe receiver is placed at the origin.
%           M   A rotation matrix to go from the alignment of the global
%               coordinate system to that at the receiver. The z vector of
%               the local coordinate system of the receiver is the pointing
%               direction of the receiver. If this matrix is omitted, then
%               the identity matrix is used.
%           xi  A 3 X numCubaturePoints matrix of cubature points for the
%               numeric integration. If this and the final parameter are
%               omitted, then fifthOrderCubPoints is used to generate
%               cubature points.
%           w   A numCubaturePoints X 1 vector of the weights associated
%               with the cubature points.
%
%OUTPUTS:   zCart   The approximate mean of the PDF of the Cartesian
%                   converted measurement in [x;y;z] Cartesian coordiantes
%           RCart   The approximate 3 X 3 covariance matrix of the PDF of
%                   the Cartesian converted measurement.
%
%Details of the conversion are given in "Basic Tracking Using Nonlinear 3D
%Monostatic and Bistatic Measurements" by David F. Crouse.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<7)
    [xi,w]=fifthOrderCubPoints(3);
end

if(nargin<6||isempty(M))
    M=eye(3);
end

if(nargin<5||isempty(zRx))
    zRx=zeros(3,1);
end

if(nargin<4||isempty(zTx))
    zTx=zeros(3,1);
end

if(nargin<3||isempty(useHalfRange))
    useHalfRange=false;
end

%Perform the conversion
numCubaturePoints=size(xi,2);

%Transform the cubature points to match the given Gaussian.
cubPoints=transformCubPoints(xi,z,SR);

%Convert all of the points into Cartesian space
for curPoint=1:numCubaturePoints
    cubPoints(:,curPoint)=ruv2Cart(cubPoints(:,curPoint),useHalfRange,zTx,zRx,M);
end

%Extract the first two moments of the transformed points.
[zCart,RCart]=calcMixtureMoments(cubPoints,w);
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

