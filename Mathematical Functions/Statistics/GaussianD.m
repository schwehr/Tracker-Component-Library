classdef GaussianD
%Functions to handle the scalar and multivariate Gaussian distribution.
%Implemented methods are: mean,cov, PDF, PDFI, PDFS, CDF, invCDF (for
%                         scalar distributions), rand, randS
%
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

methods(Static)

function val=mean(mu,Sigma)
%%MEAN  Obtain the mean of the Gaussian distribution
%
%INPUTS:    mu  The mean of the PDF. If the PDF is multivariate, then this
%               is a column vector.
%         Sigma The variance (if scalar) or covariance matrix (if
%               multidimensional) of the PDF. The variance cannot be zero
%               and the covariance matrix cannot be singular.
%
%OUTPUTS: val  The mean of the Gaussian distribution.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=mu;
end

function val=cov(mu,Sigma)
%%COV   Obtain the covariance matrix of the Gaussian distribution (the
%       variance if scalar).
%
%INPUTS:    mu  The mean of the PDF. If the PDF is multivariate, then this
%               is a column vector.
%         Sigma The variance (if scalar) or covariance matrix (if
%               multidimensional) of the PDF. The variance cannot be zero
%               and the covariance matrix cannot be singular.
%
%OUTPUTS: val  The variance of the Gaussian distribution.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=Sigma;
end

function vals=PDF(z,mu,Sigma)
%%PDF         Evaluate a scalar or multivariate Gaussian (normal) PDF at a
%             certain point given the mean and the covariance matrix.
%
%INPUTS:    z   The points at which the PDF should be evaluated. If the PDF
%               is multivariate, then this is a column vector. If
%               evaluation at multiple points are desired, then this is a
%               matrix with each column being the a point (a vector).
%           mu  The mean of the PDF. If the PDF is multivariate, then this
%               is a column vector.
%         Sigma The variance (if scalar) or covariance matrix (if
%               multidimensional) of the PDF. The variance cannot be zero
%               and the covariance matrix cannot be singular.
%
%OUTPUTS: vals  The scalar value of the normal PDF with mean mu and
%               covariance matrix Sigma evaluated at the point z. If
%               multiple points are passed (z is a matrix), then val is a
%               column vector.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
    numPoints=size(z,2);
    vals=zeros(numPoints,1);
    for curPoint=1:numPoints
        diff=z(:,curPoint)-mu;
        vals(curPoint) = (det(2*pi*Sigma))^(-1/2)*exp(-0.5*invSymQuadForm(diff,Sigma));
    end
end

function vals= PDFI(z,mu,SigmaInv)
%%PDFI        Evaluate a scalar or multivariate Gaussian (normal) PDF at a
%             certain point given the mean and the inverse of the
%             covariance matrix.
%
%INPUTS:    z   The points at which the PDF should be evaluated. If the PDF
%               is multivariate, then this is a column vector. If
%               evaluation at multiple points are desired, then this is a
%               matrix with each column being the a point (a vector).
%           mu  The mean of the PDF. If the PDF is multivariate, then this
%               is a column vector.
%           SigmaInv   The inverse of the variance (if scalar) or the
%               inverse of the covariance matrix (if multidimensional)
%               of the PDF. SigmaInv can be singular.
%
%OUTPUTS: val   The scalar value of the normal PDF with mean mu and
%               inverse covariance matrix SigmaInv evaluated at the point
%               z. If multiple points are passed (z is a matrix), then val
%               is a column vector.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    numPoints=size(z,2);
    vals=zeros(numPoints,1);
    n=size(z,1);
    
    for curPoint=1:numPoints
        diff=z(:,curPoint)-mu;
        %Note that det(A^(-1))=1/det(A) and that det(a*A)=a^n*det(A), where
        %a is a scalar and A is an nXn matrix.
        
        vals(curPoint)=sqrt((2*pi)^(-n)*det(SigmaInv))*exp(-0.5*diff'*SigmaInv*diff);
    end
end

function val=PDFS(z,mu,S)
%%GAUSSIANPDFS Evaluate a scalar or multivariate Gaussian (normal) PDF at a
%              certain point given the mean and the square root of the
%              covariance matrix.
%
%INPUTS:    z   The point at which the PDF should be evaluated. If the PDF
%               is multivariate, then this is a column vector.
%           mu  The mean of the PDF. If the PDF is multivariate, then this
%               is a column vector.
%           S   The square root of the variance (if scalar) or the
%               lower-triangular square root of the covariance matrix (if
%               multidimensional) of the PDF such that S*S'=Sigma, where
%               Sigma is the covariance matrix. S cannot be a singular
%               matrix.
%
%OUTPUTS: val   The scalar value of the normal PDF with mean mu and
%               square root covariance matrix S evaluated at the point z.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

%Note that (S*S')^(-1)=(S')^(-1)*S^(-1)
    diff=S\(z-mu);
%Note that det(S*S')=det(S)*det(S') and that det(S)=det(S') so
%det(S*S')=det(S)^2. Also, det(a*S)=a^ndet(S), where a is a scalar and S is
%an nXn matrix. Thus,
%det(2*pi*S*S')=det(sqrt(2*pi)*S)^2=(2*pi)^n*det(S)^2
    n=size(z,1);
    val = (1/((2*pi)^(n/2)*det(S)))*exp(-0.5*(diff'*diff)); 
end

function val=CDF(z,mu,var)
%%CDF Evaluate a scalar Gaussian (normal) CDF at a certain point
%     given the mean and the variance, or for a normal(0,1)
%     distribution if the mean and variance are omitted.
%
%INPUTS:    z   The point(s) at which the CDF should be evaluated.
%           mu  The mean of the distribution. If omitted with the variance, 
%               a normal (0,1) distribution is assumed.
%           var The variance of the distribution. If omitted, assumed to be
%               1.
%
%OUTPUTS:   val The scalar value(s) of the normal CDF with mean mu and
%               variance var evaluated at the point(s) z.
%
%This just uses the relation between the normal CDF and the error function
%along with the erf function in Matlab.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    if(nargin<2)
       mu=0; 
    end
    if(nargin<3)
       var=1; 
    end

    x=(z-mu)/var;
    val=(1+erf(x/sqrt(2)))/2;
end

function val=invCDF(prob,mu,var)
%%CDF Evaluate the inverse CDF of a scalar Gaussian (normal) distribution
%     at a given point. given the mean and the variance, or for a
%     normal(0,1) distribution if the mean and variance are omitted.
%
%INPUTS:    prob The probability or probabilities (0<=prob<=1) at which the 
%                argument of the CDF is desired.
%           mu  The mean of the distribution. If omitted with the variance, a normal
%               (0,1) distribution is assumed.
%           var The variance of the distribution. If omitted, assumed to be 1.
%
%OUTPUTS:   val  The argument(s) of the CDF that would give the probability
%                or probabilities in prob.
%
%This just uses the relation between the normal CDF and the error function
%along with the erfinv (inverse error function) command in Matlab.
%
%June 2015 David F. Crouse, Naval Research Laboratory, Washington D.C. 

    if(nargin<2)
       mu=0; 
    end
    
    if(nargin<3)
       var=1; 
    end

    val=var*sqrt(2)*erfinv(2*prob-1)+mu;
end


function x=rand(mu,P)
%%RAND   Generate a multivariate Gaussian random variable with a given
%        mean vector and covariance matrix.
%
%INPUTS: mu  The xDim X1 mean of the multivariate Gaussian to generate.
%        P   The xDim X xDim positive definite covariance matrix of the
%            multivariate Gaussian to generate.
%
%OUTPUT: x   A sample of an xDimX1 random variable with mean mu and
%            covariance matrix P.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    xDim=size(mu,1);
    x=chol(P,'lower')*randn(xDim,1)+mu;
end

function x=randS(mu,S)
%%RANDS   Generate a multivariate Gaussian random variable with a given
%         mean vector and lower-triangular square root covariance matrix.
%
%INPUTS: mu  The xDim X1 mean of the multivariate Gaussian to generate.
%        S   The xDim X xDim lower triangular square root covariance matrix
%            of the multivariate Gaussian to generate.
%
%OUTPUT: x   A sample of an xDimX1 random variable with mean mu and
%            covariance matrix S*S'.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    xDim=size(mu,1);
    x=S*randn(xDim,1)+mu;
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
