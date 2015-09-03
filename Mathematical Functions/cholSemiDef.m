function sqrtA=cholSemiDef(A,upperOrLower)
%%CHOLSEMIDEF  Perform Cholesky decomposition on a real, symmetric matrix
%              that is either positive definite or positive semi-definite.
%              Matlab's implementation of the Cholesky decomposition cannot
%              handle positive semi-definite matrices.
%
%INPUTS: A        A symmetric, real positive semi-definite matrix.
%   upperOrLower  A string indicating whether an upper-triangular or
%                 lower-traingular Cholesky decomposition is desired. If
%                 omitted, the default value of 'upper' is used. This can
%                 take the values 'upper' and 'lower'.
%
%OUTPUTS: sqrtA  The upper or lower-triangular Cholesky decomposition of A.
%                If 'upper' is chosen, then sqrtA'*sqrtA=A. Otherwise
%                sqrtA*sqrtA'=A.
%
%This function uses the fact that Matlab's LDL decomposition algorithm can
%handle positive semi-definite matrices. The algebraic relationship between
%an LDL decomposition and a Cholesky decomposition is then used. More
%information on the LDL decomposition of singular and ill-conditioned
%matrices is given in Chapter 4.2.7 of
%G. H. Golub and C. F. Van Loan, Matrix Computations, 4th ed. Baltimore:
%Johns Hopkins University Press, 2012.
%and chapter 4.2.3 relates the LDL decomposition to the Cholesky
%decomposition.
%
%April 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<2)
    upperOrLower='upper';
end

[L,D]=ldl(A);

%The absolute value is to handle precision limitations that would make an
%otherwise positive semi-definite matrix not positive semi-definite. The
%tria function is to deal with the fact that when A is poorly conditioned,
%Matlab will return an L that is not actually lower-triangular. Thus, while
%L*sqrt(D) is indeed a matrix square root, it is not a valid Cholesky
%decomposition. The tria function turns it into a valid Cholesky
%decomposition.
sqrtA=tria(L*sqrt(abs(D)));

switch(upperOrLower)
    case 'lower'
    case 'upper'
        sqrtA=sqrtA';
    otherwise
        error('An invalid value was entered for the upperOrLower parameter.')
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
