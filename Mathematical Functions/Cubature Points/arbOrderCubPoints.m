function [xi, w]=arbOrderCubPoints(numDim,m)
%%ARBORDERCUBPOINTS Generate n-dimensional cubature points and weights of 
%                   polynomial order 2m-1. This does not produce the 
%                   minimum number of points for the selected polynomial
%                   order.
%
%INPUTS:    numDim A positive integer specifying the dimensionality of the
%                  points to be generated.
%           m      A number such that 2m-1 is the desired order of the
%                  cubature points.
%
%OUTPUTS:   xi      A numDim X numCubaturePoints matrix containing the
%                   cubature points. (Each "point" is a vector)
%           w       A numCubaturePoints X 1 vector of the weights
%                   associated with the cubature points.
%
%This function uses the quadrature rules mentioned in Section III of
%K. Ito and K. Xiong,"Gaussian filters for non linear filtering problems,"
%IEEE Transactions on Automatic Control, vol. 45, no. 5, p. 910, May 2000.
%to generate vectors and weights for the multidimensional Gauss-Hermite
%quadrature rule.
%
%For more details on how to use these points, see the comments in the
%function fifthOrderCubPoints.m.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

numPoints=m^numDim;
xi=zeros(numDim,numPoints);
w=zeros(numPoints,1);

[xL, wL]= linQuadPoints(m);

%We now have the quadrature points for a one-dimensional integration. We
%need to take all possible groups of n of these values, allowing repeats,
%to extend it to n dimensions. This means that there are m^n possible
%vectors.
curSel=1;
selVec=zeros(numDim,1);
while(curSel<=numPoints)
    xi(:,curSel)=xL(selVec+1);
    w(curSel)=prod(wL(selVec+1));
    
    %Move on to the next set of combinations.
    selVec=arbBaseInc(selVec,m);
    curSel=curSel+1;
end

%Make sure that after precision errors everything is normalized.
w=w/sum(w);
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
