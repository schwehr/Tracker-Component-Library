function V=crossProdMat(vec,side)
%%CROSSPRODMAT  Get a matrix so that the cross product between two vectors
%               can be expressed in terms of a matrix multiplication.
%               if one wishes to evaluate vec X b as V*b, where V is a
%               matrix dependent on vec, then the side option can be
%               omitted or set to left 9the default value. If one wishes to
%               evaluate b X vec as V*b, then the side option should be set
%               to 'right'.
%
%INPUTS: vec  The 3X1 vector in a cross product relation that one wishes to
%             transform into a matrix.
%       side  An optional parameter specifying the side that vec is on in
%             the cross product relation. Possible values are
%             'left' (The default value if omitted) The cross product
%                    relation is of the form vec X b, where b is another
%                    3X1 vector.
%             'right' The cross product relation is of the form b X vec.
%
%OUTPUTS: V  If side is omitted or is 'left', then V*b is the same as v X b
%            (cross(v,b)). If side is 'right, then V*b is the same as bXv.
%
%September 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<2)
    side='left';
end

V=[0,       -vec(3),    vec(2);
   vec(3),  0,          -vec(1);
   -vec(2), vec(1),     0];

switch(side)
    case 'left'
    case 'right'
        V=V';
    otherwise
        error('Incorrect side value specified.');
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
