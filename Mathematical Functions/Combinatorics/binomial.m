function val = binomial(n,k)
%%BINOMIAL Evaluate the binomial coefficient n Choose k, the total number
%          of ways of choosing k items from a total selection of n where
%          the ordering of the selection does not matter.
%
%INPUTS:  n A nonnegative  integer indicating the number of things from 
%           which one can choose.
%         k The number of items that are be chosen. This must be between
%           zero and n, inclusive.
%
%OUTPUTS: val The value of n Choose k.
%
%Blindly computing binomial coefficients using factorials can quickly lead
%to overflow problems. This is an implementation of the algorithm in
%Y. Manolopoulos, "Binomial coefficient computation: Recursion or
%iteration?" ACM SIGCSE Bulletin, vol. 34, no. 4, pp. 65-67, Dec. 2002
%that has been modified to eliminate the recursion. Additionally, a test
%has been added such that if k>n, then zero is returned, and an additional
%test for large k has been added to use the identity that
%binomial(n,k)==binomial(n,n-k) to speed up the algorithm. Also, when the
%numbers get very large, a method using logarithmic factorials is used to
%ensure that the results are obtained quickly.
%
%September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    if(k>n)
        val=0;
        return
    end

    if(k>fix(n/2))
       k=n-k; 
    end
    
    if(n>=800&&k>=100)
        %If k is this large for large n, then numerical precision problems
        %would have already cut off a few digits of the result, so using a
        %faster but numerically less precise but faster method won't
        %matter.
        val=exp(gammaln(n+1)-gammaln(k+1)-gammaln(n-k+1));
    else
        val=1;
        for m=(k-1):-1:0
           d=gcd(n-m,k-m);%GCD = greatest common divisor
           q=(k-m)/d;
           val = (val/q)*((n-m)/d);
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
