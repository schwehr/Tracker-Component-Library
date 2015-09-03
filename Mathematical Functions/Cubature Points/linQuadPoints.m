function [xi, w]= linQuadPoints(m)
%%LINQUADPOINTS Get quadrature points of polynomial degree m for
%               integration over a scalar Gaussian random variable.
%
%INPUTS:        m A positive integer such that 2m-1 is the highest degree
%                 to which the cubature points are accurate.
%
%OUTPUTS:       xi  A 1 X numCubaturePoints vector containing the
%                   cubature points.
%               w   A numCubaturePointsX1 vector of the weights
%                   associated with the cubature points.
%
%This is an implementation of the algorithm in
%G. H. Golub and J. H. Welsh, "Calculation of Gauss quadrature rules,"
%Mathematics of Computation, vol. 23, pp. 221?230, 1969.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

J=zeros(m,m);
for k=1:m-1
   J(k,k+1)=sqrt(k);
   J(k+1,k)=sqrt(k);
end
%The eigenvectors obtained by eig are normalized.
[V,D]=eig(J);

xi=diag(D)';%The x values are the eigenvalues
%The weights are the square of the first elements of each normalized
%eigenvector.
w=abs(V(1,:)').^2;
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
