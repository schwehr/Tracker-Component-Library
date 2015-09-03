function val=FibonacciNum(n)
%%FIBONACCINUM  Determine the nth Fibonacci number. Fibonacci numbers are
%               the sequence 1, 1, 2, 3, 5, 8, ... for n=1,2,3,4,5,6,...
%               where F_{n}=F_{n-1}+F_{n-2} determines the next value. A
%               non-recursion expression is used here so that large
%               Fibonacci numbers can be quickly found.
%
%INPUTS: n   A scalar or matrix of integer Fibonacci number positions, n>0.
%
%OUTPUTS: val The values of the Fibonacci numbers at the positions given in
%             n.
%
%The non-recursive formula is taken from
%Chandra, Pravin and Weisstein, Eric W. "Fibonacci Number." From
%MathWorld--A Wolfram Web Resource.
%http://mathworld.wolfram.com/FibonacciNumber.html
%
%October 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

%The golden ratio.
phi=(1+sqrt(5))/2;

val=round(phi.^n/sqrt(5));
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
