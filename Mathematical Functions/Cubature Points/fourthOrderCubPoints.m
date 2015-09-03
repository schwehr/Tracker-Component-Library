function [sigma, w]=fourthOrderCubPoints(numDim)
%FOURTHORDERCUBPOINTS Generate fourth order cubature points using a common
%                     published method that is generally less efficient
%                     than fifth-order methods.
%
%INPUTS:    numDim  An integer specifying the dimensionality of the points
%                   to be generated.
%
%OUTPUTS:   xi      A numDim X numCubaturePoints matrix containing the
%                   cubature points. (Each "point" is a vector)
%           w       A numCubaturePoints X 1 vector of the weights
%                   associated with the cubature points.
%
%This computes the (unscaled and unshifted) sigma points with respect to
%the standard normal distribution for the fourth order unscented transform,
%which was first mentioned in 
%S. J. Julier and J. K. Uhlmann, "A consistent, debiased method for
%converting between polar and Cartesian coordinate systems," in Proceedings
%of SPIE: Acquisition, Tracking, and Pointing XI, vol. 3086, Orlando, FL,
%23?24 Oct. 1997, pp. 110?121.
%
%The implementation is the more straightforward solution given in Section
%6.2.3 of 
%U. N. Lerner, "Hybrid Bayesian networks for reasoning about complex
%systems," Ph.D. dissertation, Stanford University, Stanford, CA,
%Oct. 2002.
%
%This algorithm only works for n>=2. If n=4, then this technique works, but
%many points have zero weight. If n>4, negative weights are present and
%these points should not be used for covariance computation.
%
%In general, the cubature points in fifthOrderCubPoints or
%fifthOrderCubPointsAlt should be used in place of these. More
%
%For more details on how to use these points, see the comments in the
%function fifthOrderCubPoints.m.
%
%July 2012 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

%The numbers of different types of points.
    n=numDim;
    numType1=1;
    numType2=2*n;
    numType3=2*n*(n-1);
    numPoints=numType1+numType2+numType3;

%First, calculate the weights.
    w=zeros(numPoints,1);
    w(1)=(1/18)*(18-7*n+n^2);
    w((numType1+1):(numType1+numType2))=(4-n)/18;
    w((numType1+numType2+1):numPoints)=1/36;

%Now, calculate the sigma points.
    sigma=zeros(n,numPoints);
    
    %The first point is a type-1 point and is left at zero.
    
    %The type two points come in +/- pairs on the coordinate axes.The
    %value on the coordiante axes is
    u=sqrt(3);
    count=2;
    for idx=1:n
        sigma(idx,count)=u;
        sigma(idx,count+1)=-u;
        
        count=count+2;
    end
    
    %The type 3 points are formed by going through all possible
    %combinations of the data such that (if n>2), one element is zero and
    %all of the other elements are either u or -u. The points cover all
    %such combinations.
    
    %We need to go through all possible combinations of choosing two spots
    %in a vector of length n to place the values. the values can be +/-u.
    
    combos=nchoosek(1:n,2)';
    numCombos=size(combos,2);
    
    for curCombo=1:numCombos
        curSel=combos(:,curCombo);
        sigma(curSel,count)=[u;u];
        sigma(curSel,count+1)=[u;-u];
        sigma(curSel,count+2)=[-u;u];
        sigma(curSel,count+3)=[-u;-u];
        count=count+4;
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
