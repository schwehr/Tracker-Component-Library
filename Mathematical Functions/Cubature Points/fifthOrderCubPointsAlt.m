function [xi, w]=fifthOrderCubPointsAlt(numDim)
%FIFTHORDERCUBPOINTSALT Generate fifth order cubature points using an
%                       efficient method that does not require very many
%                       points, but which requires at least four dimensions
%                       and which produces negative weights for more than
%                       seven dimensions.
%
%INPUTS:    numDim  An integer specifying the dimensionality of the points
%                   to be generated. This must be >=4.
%
%OUTPUTS:   xi      A numDim X numCubaturePoints matrix containing the
%                   cubature points. (Each "point" is a vector)
%           w       A numCubaturePoints X 1 vector of the weights
%                   associated with the cubature points.
%
%This function returns fifth order cubature points and weights of the
%specified dimensions for Gaussian cubature integration over a normal
%distribution. The points and weights can be used to get exact solutions to
%multivariate integrals of the N(0,I) distribution times an polynomials up
%to the fifth order. When used with arbitrary functions, the points can be
%used to approximate the value of the integral. These points are generated
%according to Formula I of
%J. Lu and D. L. Darmofal, "Higher-dimensional integration with Gaussian
%weight for applications in probabilistic design," SIAM Journal on
%Scientific Computing, vol. 26, no. 2, pp. 613-624, 2004.
%Some typos were corrected by referring back to
%I. P. Mysovskikh, "The approximation of multiple integrals by using
%interpolatory cubature formulae," in Quantitative Approximation,
%R. A. DeVore and K. Scherer, Eds. New York: Academic Press, 1980,
%pp. 217-244.
%
%Using this method, fewer cubature points are needed for a given
%dimensionality than when using fifthOrderCubPoints(numDim). However, this
%approach should not be used when for numDim>7 as cubature points with
%negative weights are produced. This function requires that n>=4 whereas
%fifthOrderCubPoints only requires that n>=3. 
%
%For more details on how to use these points, see the comments in the
%function fifthOrderCubPoints.m.
%
%July 2012 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    %This only has positive weights for dimensionalities less than or
    %equal to 7.
    n=numDim;
    if(n<4)
        error('These fifth-order cubature points are only valid for dimensionalities larger than 3.')
    end
    
    numPoints=n^2+3*n+3; 
    w=zeros(numPoints,1);
    xi=zeros(n,numPoints);

    xCur=1;
    w(xCur)=2/(n+2);%The first point is the origin.

    %The next 2*(n+1) points come from calculating a-values, which must
    %be saved to calculate b-values.
    ub=2*(n+1)+xCur;
    xCur=xCur+1;
    w(xCur:ub)=n^2*(7-n)/(2*(n+1)^2*(n+2)^2);

    ar=zeros(n,n+1);
    temp=sqrt(n+2);
    for r=1:(n+1)
        for k=1:(r-1)
            ar(k,r)=-sqrt((n+1)/(n*(n-k+2)*(n-k+1)));
        end
        %for k=r;
        %This had a TYPO in Lu and Darmofal's paper that was correcting
        %using Mysovskikh paper (which itself is missing a closed
        %parenthesis in one part).
        if(r<n+1)
            ar(r,r)=sqrt((n+1)*(n-r+1)/(n*(n-r+2)));
        end
        %for all higher k, it is zero.

        xi(:,xCur)=temp*ar(:,r);
        xi(:,xCur+1)=-temp*ar(:,r);
        xCur=xCur+2;
    end

    %Now, the final n*(n+1) points come from combining all pairs of a
    %points.
    w(xCur:end)=2*(n-1)^2/((n+1)^2*(n+2)^2);

    %CORRECTED with a factor of sqrt(2) compared to the original paper.
    temp=sqrt(0.5*(n*(n+2)/(n-1)));
    for l=1:n
        for k=(l+1):(n+1)
            b=temp*(ar(:,l)+ar(:,k));
            xi(:,xCur)=b;
            xi(:,xCur+1)=-b;
            xCur=xCur+2;
        end
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
