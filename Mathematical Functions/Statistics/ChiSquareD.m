classdef ChiSquareD
%Functions to handle the central or non-central chi-square distributions.
%Implemented methods are: mean, var, PDF, CDF, invCDF (only for the central
%chi square distribution), rand
%
%DEPENDENCIES: NoncentralGammaD.m
%
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

methods(Static)

function val=mean(nu,lambda)
%%MEAN  Obtain the mean of central chi square probability distribution for
%       the given number of degrees of freedom.
%
%INPUTS: nu    The number of degrees of freedom of the central chi-square
%              distribution. Note that nu>0.
%       lambda The non-centrality parameter of the distribution. In the
%              central chi-squared distribution, this is zero.
%
%OUTPUTS: val  The mean of the central chi square distribution under
%              consideration.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=nu+lambda;
end

function val=var(nu,lambda)
%%VAR   Obtain the variance of central chi square probability distribution
%       for the given number of degrees of freedom.
%
%INPUTS: nu    The number of degrees of freedom of the central chi-square
%              distribution. Note that nu>0.
%       lambda The non-centrality parameter of the distribution. In the
%              central chi-squared distribution, this is zero.
%
%OUTPUTS: val  The variance of the central chi square distribution under
%              consideration.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=2*nu+4*lambda;
end
    
function val=PDF(x,nu,lambda)
%%PDF          Evaluate the central chi square probability distribution
%              function at a desired point.
%
%INPUTS:    x   The point(s) at which the chi-square PDF is to be 
%               evaluated. Note that x>=0.
%           nu  The number of degrees of freedom of the central chi-square
%               distribution. Note that nu>0.
%       lambda The non-centrality parameter of the distribution. In the
%              central chi-squared distribution, this is zero.
%
%OUTPUTS:   val The value(s) of the chi square PDF with nu degrees of
%               freedom evaluated at x.
%
%When lambda is very small but nonzero, the PDF function could run into
%problems due to the logarithm of lambda. The logarithms were used instead
%of a simpler form to lessen the numerical instability. If an invalid value
%for the noncentral distribution is obtained, it is assumed that it is due
%to using an extremely small nonzero value of lambda, and the result is
%approximated using lambda=0.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    if(lambda==0)
        val=(1/(2^(nu/2)*gamma(nu/2))).*x.^((nu-2)/2).*exp(-x/2);
    else
        val=0.5*exp(-(x+lambda)/2+(nu/4-0.5)*(log(x)-log(lambda)))*besseli(nu/2-1,sqrt(lambda*x));
        
        if(~isfinite(val))
            val=(1/(2^(nu/2)*gamma(nu/2))).*x.^((nu-2)/2).*exp(-x/2);
        end
    end
end

function prob=CDF(x,nu,lambda)
%%CDF          Evaluate the cumulative distribution function of the
%              central chi-square distribution at a desired point.
%
%INPUTS:    x   The point(s) at which the chi-square CDF is to be 
%               evaluated. Note that x>=0.
%           nu  The number of degrees of freedom of the central chi-square
%               distribution. Note that nu>0. When lambda is not zero, the
%               CDF function is only numerically stable over a wide range
%               of values when nu is an integer.
%       lambda  The non-centrality parameter of the distribution. In the
%               central chi-squared distribution, this is zero.
%
%OUTPUTS:   prob The value(s) of the CDF of the chi-square distribution
%                with nu degrees of freedom evaluated at x.
%
%The CDF of the central chi square function with nu degrees of freedom
%evaluated at x is just a special case of the incomplete gamma function, as
%described on Mathworld at
%http://mathworld.wolfram.com/Chi-SquaredDistribution.html
%The incomplete gamma function is built into Matlab without the use of any
%toolboxes. When the noncentrality parameter is nonzero, the CDF is
%evaluated in terms of the equivalent noncentral gamma distribution. The
%noncentral gamma algorithm is documented further in NoncentralGamma.m.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%December 2014 David A. Karnick, Naval Research Laboratory, Washington D.C.

    if(lambda==0)
        prob=gammainc(x/2,nu/2);
    else
        prob=NoncentralGammaD.CDF(x,nu/2,2,lambda);
    end
end
    
function val=invCDF(prob,nu)
%%INVCDF          Evaluate the inverse of the cumulative distribution
%                 function of the central chi square distribution.
%
%INPUTS:    prob The probability or probabilities (0<=prob<=1) at which the 
%                argument of the CDF is desired.
%           nu   The number of degrees of freedom of the central chi-square
%                distribution. Note that nu>0.
%
%OUTPUTS:   val  The argument(s) of the CDF that would give the probability
%                or probabilities in prob.
%
%The CDF of the chi square function with nu degrees of freedom evaluated
%at x is just a special case of the incomplete gamma function, as described
%on Mathworld at
%http://mathworld.wolfram.com/Chi-SquaredDistribution.html
%The inverse incomplete gamma function is built into Matlab without the use
%of any toolboxes and thus is called here.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=2*gammaincinv(prob,nu/2);
end

function vals= rand(N,nu,lambda)
%%RAND           Generate chi-square distributed random variables
%                with the given parameters.
%
%INPUTS:    N      If N is a scalar, then randChiSquare returns an NXN
%                  matrix of random variables. If N=[M,N1] is a two-element 
%                  row vector, then randChiSquare returns an MXN1 matrix of
%                  random variables.
%           nu     The number of degrees of freedom of the central
%                  chi-square distribution. Note that nu>0. If lambda is
%                  not zero, nu must be an integer.
%       lambda     The non-centrality parameter of the distribution. In the
%                  central chi-squared distribution, this is zero.
%
%OUTPUTS:   vals   A matrix whose dimensions are determined by N of the
%                  generated gamma random variables.
%
%The algorithm for the central chi squared distribution is an implementation
%of the inverse transform algorithm of chapter 5.1 of
%S. H. Ross, Simulation, Ed. 4, Amsterdam: Elsevier, 2006.
%When the noncentral distribution is used, the random variables are
%generated by summing the squares of normally distributed random variables.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    if(isscalar(N))
        dims=[N N];
    else
        dims=N;
    end

    if(lambda==0)
        U=rand(dims);
        vals=ChiSquareD.invCDF(U,nu);
    else
        mu=sqrt(lambda/nu);
        
        vals=0;
        for curSum=1:nu
            U=randn(dims);
            vals=vals+(U+mu).^2;
        end
    end
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
