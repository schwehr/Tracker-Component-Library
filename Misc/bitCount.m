function n=bitCount(intVal)
%%BITCOUNT Count the number of ones in a positive, non-negative integer
%          value less than 2^52.
%
%INPUTS: intVal  A non-negative integer value less than 2^52. Non-integers
%                are truncated and negative numbers return an error.
%
%OUTPUTS:     n  The number of one-bits in a non-floating point binary
%                representation of intVal that are set to 1.
%
%Though many creative, efficient techniques exist for counting the number
%of ones in a number when using C, this function just calls dec2bin and
%sums the result. Because Matlab's default input to functions is a
%double-precision floating point number, even if a computer supports 64-bit
%integer arithmetic, the resolution of the numbers is limited by the
%number of bits in the mantissa of a double-precision floating point
%number.
%
%September 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

%The dec2bin function returns characters, so a comparison to the character
%'1' must be performed to get actual binary numbers that can be summed.
n=sum(dec2bin(intVal)=='1');
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
