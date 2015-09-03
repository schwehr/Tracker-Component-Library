function S=tria(A)
%TRIA Square root matrix triangularization.
%
%INPUTS:    A A matrix that is generally not square.
%
%OUTPUTS:   S A square lower-triangular matrix such that S*S'=A*A'.
%
%This is the tria function need for various steps in the cubature Kalman
%filter and the square root Kalman filter. It is described in "Basic
%Tracking Using Nonlinear 3D Monostatic and Bistatic Measurements" by David
%F. Crouse. It has been slightly modified from the paper so that the
%diagonal elements remain positive.
%
%July 2012 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    [~,R]=qr(A',0);
    S=R';
    
    %Make the diagonal elements all positive.
    sel=diag(S)<0;
    S(:,sel)=-S(:,sel);
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
