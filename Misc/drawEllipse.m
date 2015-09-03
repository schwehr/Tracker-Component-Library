function drawEllipse(z,A,gamma,varargin)
%DRAWELLIPSE Draw 2D ellipses on the curren plot, where a point zp on the
%            ellipse satisfies the equation (zp-z)'*A*(zp-z)=gamma. If
%            z is a 2D location estimate and inv(A) a Gaussian covariance
%            matrix associated with the estimate, then the ellipse is a
%            probability region, where gamma determines what amount of
%            probability is in the region. If omitted, gamma=16.2, which
%            corresponds to approximately to the 99.97% confidence region
%            is used (The value comes from ChiSquareD.invCDF(0.9997,2)).
%
%INPUTS: z  A 2XN vector corresponding to the centers of the N ellipses
%           that should be drawn.
%        A  A 2X2XN set of N positive definite matrices that specify the
%           size and shape of the ellipse, where a point zp is on the
%           ellipse if (zp-z)'*A*(zp-z)=gamma.
%     gamma An optional parameter specifying the size of the ellipse. If
%           omitted, then it will be assumed that inv(A) has the form of a
%           covariance matrix and gamma=16.2 is used, which corresponds to
%           an ellipse of approximately 99.97% confidence interval. gamma
%           must be positive.
%  varargin Sets of values that should be passed to the plot function to
%           format the ellipses. For example, one could call the function
%           as drawEllipse(z,A,gamma,'--r','linewidth',2) to plot the
%           ellipses as thick, red lines. Note that Matlab will not always
%           properly render dashed lines due to the number of points used
%           to plot the shape.
%
%OUTPUTS: None. The results are plotted.
%
%An eigendecomposition of a matrix A breaks it into parts V and D such that
%V*D*V'=A, where V is a rotation matrix and D is a diagonal matrix.
%Considering the problem here, this means that (zp-z)'*A*(zp-z)=
%(V'*zp-V'*z)'*D*(V'*zp-V'*z). If we substitute kp=V'*zp and k=V'z, then
%the equation is (kp-k)'*D*(kp-k)=gamma, which is the equation for an
%ellipse where the axes are aligned with the coordinate axes. Thus, one can
%first find the points for an ellipse that is aligned with the coordinate
%axes (the kp points), and then rotate it back to the proper alignment (the
%zp points). However, it is simpler to first find an ellipse centered about
%zero. Thus, substitute l=kp-k and find the l points, then shift and rotate
%them to get the z-points.
%
%Suppose that l=[x;y] and D=diag(1/a,1/b). Then the ellipse equation is
%x^2/a+y^2/b=gamma
%Solving for y in terms of x, we have
%y=+/-sqrt(b*(gamma-x^2/a)).
%The x values are limited so that the argument of the square root is
%positive. Thus the x values range from -sqrt(a*gamma) to +sqrt(a*gamma).
%Thus, a simple way to plot this centered, axis-aligned ellipse is to
%generate x-values in the valid range and then find the two sets of y
%values. This function does that and transforms the results to get the
%corresponding z values. 1000 points are used in the plot.
%
%July 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<3)
    gamma=16.2;
end

numPoints=1000;
numEllipse=size(z,2);

%Save the value of hold on the plot so that it can be reset to its previous
%state after plotting all of the ellipses.
holdVal= ishold();

for curEllip=1:numEllipse
    %Perform an eigenvalue decomposition of A.
    [V,D]=eig(A(:,:,curEllip));

    a=1/D(1,1);
    b=1/D(2,2);

    xBound=sqrt(a*gamma);
    x=linspace(-xBound,xBound,numPoints);
    %The real command deals with possible finite precision issues.
    y=real(sqrt(b*(gamma-x.^2/a)));
    %The centered, axis-aligned ellipse points.
    l=[x,fliplr(x);
       y,-fliplr(y)];
    zp=bsxfun(@plus,V*l,z(:,curEllip));
    
    %Make sure that all of the ellipses are printed.
    if(curEllip~=1)
        hold on
    end
    plot(zp(1,:),zp(2,:),varargin{:});
end

if(~holdVal)
    hold off
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
