function [xi, w]=fifthOrderCubPoints(numDim)
%FIFTHORDERCUBPOINTS Generate fifth order cubature points using a technique
%                    that is more efficient than that given by 
%                    arbOrderCubPoints, but less efficient than that given
%                    by fifthOrderCubPointsAlt. Unlike
%                    fifthOrderCubPointsAlt, the weights here will always
%                    be positive.
%
%INPUTS:    numDim  An integer specifying the dimensionality of the points
%                   to be generated. This must be >2.
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
%according to Algorithm 5-3 on page 317 of the book
%A.H. Stroud, Approximate Calculation of Multiple Integrals. Cliffs, NJ:
%Prentice-Hall, Inc., 1971.
%Note that the state must be at least three dimensional.
%
%If f() if a function that returns the value for a function at a given
%point, then the integral of the N(0,I) times that function over the entire
%real space can be approximated using the points as
%
%intVal=0;
%for(i=1:numCubaturePoints)
%   intVal=intVal+w(i)*f(x(:,i));
%end
%
%If an integral over a function times N(mu,Sigma) is desired instead of
%over a function times N(0,I), the points should be modified as
%x=bsxfun(@plus,chol(Sigma,'lower')*x,mu);
%The weights remain unchanged.
%
%July 2012 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    n=numDim;
    if(n<=2)
        error('These fifth-order cubature points are only valid for dimensionalities larger than 2.') 
    end
    
    numPoints=2^n+2*n;

    w=zeros(numPoints,1);
    w(1:(2*n))=4/(n+2)^2;
    w((2*n+1):numPoints)=(n-2)^2/(2^n*(n+2)^2);

    %Allocate space
    xi=zeros(n,2*n);

    %First, go through all of the positions of r:
    r=sqrt((n+2)/2);

    %First, the positive values
    for k=1:n
        xi(k,k)=r; 
    end

    %Now, the negative values
    for k=(n+1):(2*n)
        xi(k-n,k)=-r;
    end

    %Now, go through all of the positions of s.
    s=sqrt((n+2)/(n-2));
    xi=[xi PMCombos(ones(n,1)*s,0)];
end

function val=PMCombos(x,zp)
    %This goes through all possible combinations of making the elements in
    %x positive or negative. A gray code would probably be faster. The zp
    %zero pads the result by the given number of zeros.
    n=length(x);
    numPerm=2^n;

    val=zeros(n+zp,numPerm);
    curMult=ones(n,1)*(-1);

    val(1:n,1)=x.*curMult;

    for curItem=2:numPerm
        %Increment the permutation number.
        idx=1;
        while(curMult(idx)==1)
            curMult(idx:-1:1)=-1;
            idx=idx+1;
        end
        curMult(idx)=1;
        val(1:n,curItem)=x.*curMult;
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
