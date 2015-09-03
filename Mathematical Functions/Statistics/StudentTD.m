classdef StudentTD
%Functions to handle the multivariate Student-t distribution.
%Implemented methods are: mean, cov, PDF
%
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

methods(Static)
    
function val=mean(mu,Sigma,nu)
%%MEAN  Obtain the mean of the multivariate Student-t distribution for 
%       given location and scale parameters and degrees of freedom.
%
%INPUTS: mu     The DX1 location vector of the student's-t distribution.
%       Sigma   The DXD symmetric, positive definite scale matrix.
%        nu     The number of degrees of freedom of the Student-t
%               distribution.
%
%OUTPUTS: val  The mean of the multivariate Student-t distribution.
%
%The mean is undefined if nu<=1.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
        
    if(nu>1)
        val=mu;
    else
        val=NaN;
    end
end 

function val=cov(mu,Sigma,nu)
%%MEAN  Obtain the covariance matrix of the multivariate Student-t 
%       distribution for given location and scale parameters and degrees 
%       of freedom.
%
%INPUTS: mu     The DX1 location vector of the student's-t distribution.
%       Sigma   The DXD symmetric, positive definite scale matrix.
%        nu     The number of degrees of freedom of the Student-t
%               distribution.
%
%OUTPUTS: val  The covariance matrix of the multivariate Student-t
%              distribution.
%
%The covariance matrix is undefined if nu<=2.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
        
    if(nu>2)
        val=nu/(nu-2)*Sigma;
    else
        val=NaN;
    end
end 
    
function val=PDF(x,mu,Sigma,nu)
%%PDF  Evaluate the monovariate or multivariate Student-t
%              distribution at the desired point.
%
%INPUTS: x      The DX1 vector at which the (possibly multivariate)
%               Student's-t distribution is to be evaluated.
%        mu     The DX1 location vector of the student's-t distribution.
%       Sigma   The DXD symmetric, positive definite scale matrix.
%        nu     The number of degrees of freedom of the Student-t
%               distribution.
%
%OUTPUTS: val   The PDF of the Student's-t distribution at x with the given
%               parameters.
%
%The vector version of the Student-t Distribution is given in Appendix B of 
%C. M. Bishop, Pattern Recognition and Machine Learning. Cambridge,
%United Kingdom: Springer, 2007.
%As nu->Inf, the distribution reduces to a multivariate Gaussian
%distribution with mean mu and covariance matrix Sigma.
%
%Logarithms are used in the implementation to reduce the effect of
%precision problems that can arise if nu is large. The problems arise due 
%to the ratio of gamma functions.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    D=size(x,1);

    diff=x-mu;
    Delta2=diff'*pinv(Sigma)*diff;

    num=gammaln((nu+D)/2);
    denom=gammaln(nu/2)+log(sqrt(det(nu*pi*Sigma)))+((D+nu)/2)*log(1+Delta2/nu);

    val=num-denom;
    val=exp(val);
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
