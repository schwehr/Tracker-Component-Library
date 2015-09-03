function [xi,w]=seventhOrderCubPoints(numDim)
%%SEVENTHORDERCUBPOINTS Generate seventh order cubature points for
%                       integration involving a multidimensional Gaussian
%                       probability density function (PDF). Note that if
%                       numDim=8, then 32 of the points will have zero
%                       weights (and are thus eliminated) and if numDim>8,
%                       then some points will have negative weights.
%
%INPUTS:    numDim  An integer specifying the dimensionality of the points
%                   to be generated. This must be >2.
%
%OUTPUTS:   xi      A numDim X numCubaturePoints matrix containing the
%                   cubature points. (Each "point" is a vector)
%           w       A numCubaturePoints X 1 vector of the weights
%                   associated with the cubature points.
%
%These points are generated according to Algorithm 7-3 on page 319 of the 
%book
%A.H. Stroud, Approximate Calculation of Multiple Integrals. Cliffs, NJ:
%Prentice-Hall, Inc., 1971.
%Note, however, that the formula in the book contains a type. Using 
%A. H. Stroud, "Some seventh degree integration formulas for symmetric
%regions," SIAM Journal on Numerical Analysis, vol. 4, no. 1, pp. 37-44,
%Mar. 1967.
%and 
%A. H. Stroud, "Some seventh degree integration formulas for the surface of
%an n-sphere," Numerische Mathematik, vol. 11, no. 3, pp. 273-276, Mar.
%1968.
%The steps in the corrected algorithm are summarized in the paper
%D. F. Crouse, "Basic tracking using nonlinear 3D monostatic and bistatic
%measurements," IEEE Aerospace and Electronic Systems Magazine, vol. 29,
%no. 8, Part II, pp. 4-53, Aug. 2014.
%
%%For more details on how to use these points, see the comments in the
%function fifthOrderCubPoints.m.
%
%July 2012 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    n=numDim;
    
    if(n<3)
       error('These seventh-order cubature points are only valid for dimensionalities larger than 2.') 
    end

    %First, we will generate points for the n-sphere.
    %Allocate space.
    numUPoints=2^n+2*n^2;
    u=zeros(n,numUPoints);

    k1=1;
    k2=2;
    k3=n;

    A1=(8-n)/(n*(n+2)*(n+4));
    A2=4/(n*(n+2)*(n+4));
    An=2^(-n)*n^3/(n*(n+2)*(n+4));

    %The first component.
    r1=sqrt(1/k1);
    curEl=1;
    for curIdx=1:n
        u(curIdx,curEl)=r1;
        u(curIdx,curEl+1)=-r1;
        curEl=curEl+2;
    end

    %The second component has two nonzero entities.
    r2=sqrt(1/k2);
    for curIdx1=1:(n-1)
        for curIdx2=(curIdx1+1):n
            sel=[curIdx1;curIdx2];
            u(sel,curEl)=[r2;r2];
            u(sel,curEl+1)=[r2;-r2];
            u(sel,curEl+2)=[-r2;r2];
            u(sel,curEl+3)=[-r2;-r2];

            curEl=curEl+4;
        end
    end

    %The final component is all +/- values of all of the entities being
    %nonzero.
    r3=sqrt(1/k3);
    vec=ones(n,1)*r3;
    u(:,curEl:end)=PMCombos(vec,0);

    %Let us make the weight vector that goes along with u.
    wu=zeros(numUPoints,1);
    delta1=2*n;
    delta2=2*n*(n-1);
    delta3=2^n;
    wu(1:delta1)=A1;
    wu(delta1+(1:delta2))=A2;
    wu(delta1+delta2+(1:delta3))=An;

    %Finally, all of this must be transformed as described in Stroud's
    %book to make it work with the desired Gaussian weighting function.
    r1=sqrt((n+2-sqrt(2*(n+2)))/2);
    r2=sqrt((n+2+sqrt(2*(n+2)))/2);

    %The denominators were wrong in the paper if we want them to sum to
    %1. They are correct here.
    A(1)=(n+2+sqrt(2*(n+2)))/(2*(n+2));
    A(2)=(n+2-sqrt(2*(n+2)))/(2*(n+2));

    w=zeros(2*numUPoints,1);
    xi=zeros(n,2*numUPoints);

    w(1:numUPoints)=wu*A(1);
    w(numUPoints+(1:numUPoints))=wu*A(2);

    xi(:,1:numUPoints)=r1*u;
    xi(:,numUPoints+(1:numUPoints))=r2*u;

    xi=sqrt(2)*xi;%To make it approximate the standard normal distribution.
    
    if(numDim==8)
        %Get rid of the zero weights when numDim==8.
        sel=(w~=0);
        w=w(sel);
        xi=xi(:,sel);
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
