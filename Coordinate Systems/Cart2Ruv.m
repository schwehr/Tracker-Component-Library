function z=Cart2Ruv(zC,useHalfRange,zTx,zRx,M)
%CART2RUV Convert points in Cartesian coordiantes (either the global
%         system or a local system at the receiver) into local bistatic
%         r-u-v coordinates of the receiver. 
%
%INPUT: zC  A 3XN matrix of Cartesian points in global [x;y;z] Cartesian
%           coordinates.
%useHalfRange A boolean value specifying whether the bistatic range value
%           should be divided by two. This normally comes up when operating
%           in monostatic mode, so that the range reported is a one-way
%           range. The default if this parameter is not provided is false.
%       zTx The 3XN [x;y;z] location vectors of the transmitters in global
%           Cartesian coordinates. If this parameter is omitted, then the
%           transmitters are assumed to be at the origin. If only a single
%           vector is passed, then the transmitter location is assumed the
%           same for all of the target states being converted. zTx can have
%           more than 3 rows; additional rows are ignored.
%       zRx The 3X1 [x;y;z] location vector of the receiver in Cartesian
%           coordinates.  If this parameter is omitted, then the
%           receivers are assumed to be at the origin. If only a single
%           vector is passed, then the receiver location is assumed the
%           same for all of the target states being converted. zRx can have
%           more than 3 rows; additional rows are ignored.
%      M    A 3X3XN hypermatrix of the rotation matrices to go from the
%           alignment of the global coordinate system to that at the
%           receiver. The z-axis of the local coordinate system of the
%           receiver is the pointing direction of the receiver. If omitted,
%           then it is assumed that the local coordinate system is aligned
%           with the global and M=eye(3) --the identity matrix is used. If
%           only a signle 3X3 matrix is passed, then is is assumed to be
%           the same for all of the N conversions.
%
%OUTPUT: z  The 3XN matrix of location vectors of the points in bistatic
%           [r;u;v] coordinates. If useHalfRange=true, then the r component
%           is half the bistatic range.
% 
%Details of the conversion are given in "Basic Tracking Using
%Nonlinear 3D Monostatic and Bistatic Measurements" by David F. Crouse.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    N=size(zC,2);

    if(nargin<5)
        M=repmat(eye(3),[1,1,N]);
    elseif(size(M,3)==1)
        M=repmat(M,[1,1,N]);
    end

    if(nargin<4)
        zRx=zeros(3,N);
    elseif(size(zRx,2)==1)
        zRx=repmat(zRx,[1,N]);
    end

    if(nargin<3)
        zTx=zeros(3,N);
    elseif(size(zTx,2)==1)
        zTx=repmat(zTx,[1,N]);
    end
    
    if(nargin<2)
        useHalfRange=false;
    end
    
    %Allocate space for the return values.
    z=zeros(3,N);
    for curPoint=1:N
        %The target location in the receiver's coordinate system.
        zCL=M(:,:,curPoint)*(zC(:,curPoint)-zRx(:,curPoint));
        %The transmitter location in the receiver's local coordiante system.
        zTxL=M(:,:,curPoint)*(zTx(:,curPoint)-zRx(:,curPoint));

    %Perform the conversion.
        r1=norm(zCL);%Receiver to target.
        r2=norm(zCL-zTxL);%Target to transmitter.

        r=r1+r2;

        u=zCL(1)/r1;
        v=zCL(2)/r1;

        z(:,curPoint)=[r;u;v];
    end
    
    if(useHalfRange)
        z(1,:)=z(1,:)/2;
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
