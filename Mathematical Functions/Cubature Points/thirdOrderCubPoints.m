function [xi, w]=thirdOrderCubPoints(numDim)
%THIRDORDERCUBPOINTS Generate third order cubature points.
%
%INPUTS:    numDim  An integer specifying the dimensionality of the points
%                   to be generated.
%
%OUTPUTS:   xi      A numDim X numCubaturePoints matrix containing the
%                   cubature points. (Each "point" is a vector)
%           w       A numCubaturePoints X 1 vector of the weights
%                   associated with the cubature points.
%
%This function generates the third-order cubature points and weights that
%are derived in
%I. Arasaratnam and S. Haykin, "Cubature Kalman filters," IEEE Transactions
%on Automatic Control, vol. 54, no. 6, pp. 1254?1269, Jun. 2009.
%
%For more details on how to use these points, see the comments in the
%function fifthOrderCubPoints.m.
%
%July 2012 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    xi=zeros(numDim,2*numDim);
    
    curVal=1;
    for curE=1:numDim
        e=zeros(numDim,1);
        e(curE)=sqrt(numDim);
        
        xi(:,curVal)=e;
        xi(:,curVal+1)=-e;
        
        curVal=curVal+2;
    end
    
    w=ones(2*numDim,1)/(2*numDim);
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
