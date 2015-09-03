classdef PoissonD
%Functions to handle the Poisson distribution.
%Implemented methods are: mean, var, PMF, CDF, rand
%
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

methods(Static)

function val=mean(lambda)
%%MEAN  Obtain the mean of the Poisson distribution for the specified mean
%       parameter
%
%INPUTS: lambda  The mean (and variance) of the Poisson distribution 
%                under consideration.
%
%OUTPUTS: val  The mean of the Poisson distribution under consideration.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=lambda;
end

function val=var(lambda)
%%VAR   Obtain the variance of the Poisson distribution for the specified
%       mean parameter
%
%INPUTS: lambda  The mean (and variance) of the Poisson distribution 
%                under consideration.
%
%OUTPUTS: val  The variance of the Poisson distribution under
%              consideration.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=lambda;
end

function val=PMF(x,lambda)
%%PMF        Evaluate the Poisson probability mass function at a given
%            point with a specified mean parameter.
%
%INPUTS:    x       The point(s) at which the Poisson PMF is to be 
%                   evaluated. x is an integer greater than or equal to
%                   zero.
%           lambda  The mean (and variance) of the Poisson distribution 
%                   under consideration.
%
%OUTPUTS:   val The value(s) of the Poisson PMF with mean lambda.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    val=(lambda.^x./factorial(x))*exp(-lambda);
end

function [F,p]=CDF(x,lambda)
%%CDF        Evaluate the cumulative distribution function of the
%            Poisson distribution at a desired point.
%
%INPUTS:    x       The point at which the Poisson CDF is evaluated.
%           lambda  The mean (and variance) of the Poisson distribution 
%                   under consideration.
%
%OUTPUTS:   F       The CDF of the Poisson distribution evaluated at the
%                   desired point.
%           p       The value of the PMF of the Poisson distribution at the
%                   desired point.
%
%The recursion is from Chapter 4.2 of
%S. H. Ross, Simulation, Ed. 4, Amsterdam: Elsevier, 2006.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    p=exp(-lambda);
    F=p;
    for I=1:fix(x)
        p=p*lambda/I;
        F=F+p;
    end
end
    
function vals=rand(N,lambda)
%%RAND Generate Poisson random variables with a given mean.
%
%INPUTS:    N      If N is a scalar, then rand returns an NXN matrix
%                  of random variables. If N=[M,N1] is a two-element row 
%                  vector, then rand returns an MXN1 matrix of 
%                  random variables.
%           lambda The mean (and variance) of the Poisson distribution 
%                  under consideration. 
%
%OUTPUTS:   vals   A matrix whose dimensions are determined by N of the
%                  generated Poisson random variables.
%
%The Poisson random variables are generated according to the the simple
%method given in Chapter 4.2 of 
%S. H. Ross, Simulation, Ed. 4, Amsterdam: Elsevier, 2006.
%The algorithm can handle small and large values of lambda. However, there
%is always a possibility that the value desired might be on the tail
%end of the distribution and be very slow. Thus, the number of iterations
%(the offset from the mean) is set to the maximum of 10 and 10* the
%variance.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.

    if(isscalar(N))
        dims=[N N];
    else
        dims=N;
    end

    maxIter=max(10,10*lambda);

    vals=zeros(dims);
    for curRow=1:dims(1)
        for curCol=1:dims(2)
            I=fix(lambda);
            [F,p]=PoissonD.CDF(I,lambda);
            U=rand(1);

            if(U<=F)
            %This means that the random number being generated is less than or  
            %equal to I and we must search downward.
                while(U<F&&I>-1)
                    F=F-p;
                    p=p*(I/lambda);
                    I=I-1;
                end
                I=I+1;
            else
            %This means that the random number being generated is greater than 
            %I and we must search upward starting from I+1.
                curIter=0;
                while(U>F&&curIter<maxIter)
                    p=p*lambda/(I+1);
                    F=F+p;
                    I=I+1;
                    curIter=curIter+1;
                end
            end

            vals(curRow ,curCol)=I;
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
