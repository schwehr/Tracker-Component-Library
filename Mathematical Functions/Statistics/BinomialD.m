classdef BinomialD
%Functions to handle the binomial distribution.
%Implemented methods are: mean, var, PMF, CDF
%
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

methods(Static)

function val=mean(n,p)
%%MEAN  Obtain the mean of the binomial distribution for the given number
%       of trials and probability of success.
%
%INPUTS: n  The number of trials of which k successes are observed.
%        p  The probability of success for the underlying Bernoulli trials.
%           0<=p<=1.
%
%OUTPUTS: val  The mean number of successes for the binomial distribution.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=n*p;
end

function val=var(n,p)
%%VAR   Obtain the variance of the binomial distribution for the given
%       number of trials and probability of success.
%
%INPUTS: n  The number of trials of which k successes are observed.
%        p  The probability of success for the underlying Bernoulli trials.
%           0<=p<=1.
%
%OUTPUTS: val  The variance of the binomial distribution with the given
%              parameters.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=n*p*(1-p);
end

function val=PMF(k,n,p)
%%PMF         Evaluate the binomial probability mass function at a given
%             point with a specified number of trials and success
%             probability per trial.
%
%INPUTS: k  The nonnegative integer point(s) at which the binomial PMF is
%           to be evaluated (the number of successes in n trials). Note
%           that 0<=k<=n.
%        n  The number of trials of which k successes are observed.
%        p  The probability of success for the underlying Bernoulli trials.
%           0<=p<=1.
%
%OUTPUT: val The value(s) of the binomial PMF with success probability p.
%
%The binomial PMF provides the probability of k successes from n trials,
%each with a success probability of p.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    numPoints=length(k);
    val=zeros(size(k));
    for curK=1:numPoints
        val(curK)=binomial(n,k(curK))*p^k(curK)*(1-p)^(n-k(curK));
    end
end

function val=CDF(k,n,p)
%%CDF         Evaluate the cumulative distribution function of the
%             binomial distribution at a desired point.
%
%INPUTS: k  The nonnegative integer point(s) at which the binomial CDF is
%           to be evaluated. Note that 0<=k<=n.
%        n  The number of trials of which k or fewer successes are
%           observed.
%        p  The probability of success for the underlying Bernoulli trials.
%           0<=p<=1.
%
%OUTPUTS: val  The CDF of the binomial distribution evaluated at the
%              desired point(s).
%
%Rather than summing over the values of the PMF, the equivalency between
%the binomial distribution and the regularized incomplete beta function,
%described at
%http://mathworld.wolfram.com/BinomialDistribution.html
%is used. Thus, the built-in function betainc can be used for efficient
%evaluation of the CDF with large values.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=betainc(1-p,n-k,k+1);
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
