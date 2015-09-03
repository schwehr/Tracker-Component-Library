function [zCart,RCart]=pol2CartCubature(polPoint,SR,zRx,xi,w)
%%POL2CARTCUBATURE Use cubature integration to approximate the moments of a
%          noise-corrupted polar location converted into 2D Cartesian
%          coordinates. The polar angle (azimuth) is measured from the
%          x-axis counterlockwise. This function ignores all propagation
%          effects, such as atmospheric refraction. The range rate does not
%          include special relativistic effects.
%
%INPUTS: polPoint A 2XN set of N polar points of the form [range;azimuth]
%                 that are to be converted into Cartesian coordinates. The
%                 range is one-way monostatic.
%           SR  The 2X2XN lower-triangular square roots of the covariance
%               matrices associated with polPoint. If all of the matrices
%               are the same, then this can just be a single 2X2 matrix.
%           zRx The 2X1 [x;y] location vector of the receiver in
%               Cartesian coordinates.
%           xi  A 2 X numCubaturePoints (for position-only) matrix of
%               cubature points for the numeric integration. If this and
%               the final parameter are omitted, then
%               arbOrderCubPoints is used to generate fifth-order cubature
%               points.
%           w   A numCubaturePoints X 1 vector of the weights associated
%               with the cubature points.
%
%OUTPUTS: zCart  The approximate mean of the PDF of the polar-converted
%                measurement in [x;y] Cartesian coordiantes.
%         RCart  The approximate 2 X 2 covariance matrix of estimate.
%
%Details of the numerical integration used in the conversion are given in
%"Basic Tracking Using Nonlinear 3D Monostatic and Bistatic Measurements"
%by David F. Crouse.
%
%May 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    numDim=size(polPoint,1);
    numPoints=size(polPoint,2);
    
    if(numDim~=2)
        error('The polar points have the wrong dimensionality')
    end
    
    if(size(SR,3)==1)
        SR=repmat(SR,[1,1,numPoints]);
    end

    if(nargin<3||isempty(zRx))
        zRx=zeros(numDim,1);
    end
    
    if(nargin<5)
       [xi,w]=arbOrderCubPoints(numDim,3);
    end
    
    %Allocate space for the return variables.
    zCart=zeros(numDim,numPoints);
    RCart=zeros(numDim,numDim,numPoints);
    
    for curPoint=1:numPoints
        %Transform the cubature points to match the given Gaussian. 
        cubPoints=transformCubPoints(xi,polPoint(:,curPoint),SR(:,:,curPoint));

        %Transform the points
        CartPoints=pol2Cart(cubPoints(1:2,:));
        
        %Extract the first two moments of the transformed points.
        [zCart(:,curPoint),RCart(:,:,curPoint)]=calcMixtureMoments(CartPoints,w);
        
        %Deal with the offset from the origin.
        zCart(:,curPoint)=zCart(:,curPoint)+zRx;
    end
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
