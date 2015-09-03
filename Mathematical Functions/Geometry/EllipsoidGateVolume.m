function V=EllipsoidGateVolume(P,PD)
%%ELLIPSOIDGATEVOLUME  Return the volume of an ellipsoidal gate given a
%                      covariance matrix. It is assumed that the elements
%                      in the covariance matrix all have the same units,
%                      i.e. position, velocity, etc.
%
%INPUTS:    P   A covariance matrix.
%           PD  The confidence region probability around the covariance
%               matrix.
%
%OUTPUTS:   V   The scalar volume of the gating region specified by the 
%               given covariance matrix and confidence region probability.
%
%For a vector x and an estimate xHat, this function provides the volume of
%the ellipsoidal region defined by the inequality
%(x-xHat)'*inv(P)*(x-xHat)<g
%where gamma is the value of the inverse Chi-square distribution with n
%degrees of freedom evaluated at PD, where n is the dimensionality of x.
%
%The formula for the volume of an ellipsoidal gating region is from Chapter
%2.3.2 of
%Y. Bar-Shalom, P. K. Willett, and X. Tian, Tracking and Data Fusion.
%Storrs, CT: YBS Publishing, 2011.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

n=size(P,1);
g=invChiSquareCDF(PD,n);

V=(pi^(n/2)/gamma(n/2+1))*sqrt(det(g*P));
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
