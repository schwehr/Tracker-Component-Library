function M=findRFTransParam(plhPoint,az,el,a,f)
%%GETENUAXES Find the transformation matrix needed to rotate a global
%            Cartesian vector to local radar-facing coordinates.
%
%INPUTS:   plhPoint The point at which the axes are to be found given in
%                   terms of [latitude;longitude] with the geodetic
%                   latitude and longitude. The latitude should be between
%                   -pi/2 and pi/2. A height term can also be included, but
%                   it will be ignored.
%          az       The radar azimuth, in radians. If not provided, a value
%                   of zero is assumed.
%          el       The radar elevation, in radians. If not provided, a
%                   value of zero is assumed.
%           a       The semi-major axis of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84SemiMajorAxis is used.
%           f       The flattening factor of the reference ellipsoid. If
%                   this argument is omitted, the value in
%                   Constants.WGS84Flattening is used.
%
%OUTPUTS:   M       A rotation matrix for the transformation from global
%                   global cartesian coordinates to local radar-facing
%                   coordinates. This can be directly fed into the RUV
%                   coordinate transform functions.
%
%Details of this transformation can be found in Chapter 5 of "The Aries
%Program: Coordinates, transformations, trajectories and tracking", B.L.
%Diamond, et al, MIT, Sept. 1975
%
%August 2015 David Karnick, Naval Research Laboratory, Washington D.C.

if(nargin<5)
    f=Constants.WGS84Flattening;
end
if(nargin<4)
    a=Constants.WGS84SemiMajorAxis;
end
if(nargin<3)
    el=0;
end
if(nargin<2)
    az=0;
end

%Cartesian ECEF to ENU
u=getENUAxes(plhPoint,false,a,f);

%Cartesian ENU to radar-face XYZ
A = [-cos(az)         sin(az)           0
     -sin(az)*sin(el) -cos(az)*sin(el)  cos(el)
     sin(az)*cos(el)  cos(az)*cos(el)	sin(el)];
   
M = A*u.';

