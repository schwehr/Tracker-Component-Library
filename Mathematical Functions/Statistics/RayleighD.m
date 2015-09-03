classdef RayleighD
%Functions to handle the Rayleigh distribution.
%Implemented methods are: mean, var, PDF, CDF, invCDF, rand
%
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

methods(Static)
    
function val=mean(sigma)
%%MEAN  Obtain the mean of the Rayleigh distribution for a given parameter.
%
%INPUTS:    sigma  The parameter of the Rayleigh distribution.
%
%OUTPUTS: val  The mean of the Rayleigh distribution.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
    
    val=sigma*sqrt(pi/2);
end

function val=var(sigma)
%%VAR   Obtain the variance of the Rayleigh distribution for a given
%       parameter.
%
%INPUTS:    sigma  The parameter of the Rayleigh distribution.
%
%OUTPUTS: val  The variance of the Rayleigh distribution.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
    
    val=(2-pi/2)*sigma^2;
end  
    
function val=PDF(x,sigma)
%%PDF          Evaluate the Rayleigh probability distribution
%              function at one or more desired points.
%
%INPUTS:    x   The point(s) at which the Rayleigh PDF is to be 
%               evaluated. Note that x>=0.
%           sigma  The parameter of the Rayleigh distribution.
%
%OUTPUTS:   val The value(s) of the Rayleigh PDF with parameter sigma
%               evaluated at x.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=(x/sigma^2).*exp(-x.^2/(2*sigma^2));
end

function val=CDF(x,sigma)
%%CDF          Evaluate the Rayleigh probability distribution
%              function at one or more desired points.
%
%INPUTS:    x   The point(s) at which the Rayleigh CDF is to be 
%               evaluated. Note that x>=0.
%           sigma  The parameter of the Rayleigh distribution.
%
%OUTPUTS:   val The value(s) of the Rayleigh CDF with parameter sigma
%               evaluated at x.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=1-exp(-x.^2/(2*sigma^2));
end

function val=invCDF(prob,sigma)
%%INVCDF         Evaluate the inverse of the cumulative distribution
%                function of the Rayleigh distribution.
%
%INPUTS:    prob The probability or probabilities (0<=prob<=1) at which the 
%                argument of the CDF is desired.
%           sigma The parameter of the Rayleigh distribution.
%
%OUTPUTS:   val  The argument(s) of the CDF that would give the probability
%                or probabilities in prob.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=sigma*sqrt(-2*log(1-prob));
end

function vals=rand(N,sigma)
%%RAND Generate Rayleigh random variables with a given parameter sigma.
%
%INPUTS:    N      If N is a scalar, then randRayleigh returns an NXN
%                  matrix of random variables. If N=[M,N1] is a two-element 
%                  row vector, then randRayleigh returns an MXN1 matrix of 
%                  random variables.
%           sigma  The parameter of the Rayleigh distribution.
%
%OUTPUTS:   vals   A matrix whose dimensions are determined by N of the
%                  generated Rayleigh random variables.
%
%This generates Rayleigh distributed random variables by transforming 
%normally distributed random variables using the identity given in
%Chapter 2.1.4 of
%J. G. Proakis, Digital Communication. Ed. 4, Boston, MA:
%McGraw Hill, 2001.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    if(isscalar(N))
        dims=[N N];
    else
        dims=N;
    end

    X=sigma*randn(dims);
    Y=sigma*randn(dims);

    vals=sqrt(X.^2+Y.^2);
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
