function [N,rho,T,P]=standardAtmosParam(Jul1,Jul2,point,rhow)
%%STANDARDATMOSPARAM  Get basic parameters for atmospheric refractivity,
%                     density, pressure and temperature from
%                     a standard atmospheric model. The model is best
%                     suited for altitudes below 90km.
%
%INPUTS:    Jul1, Jul2  Two parts of a pseudo-Julian date given in UTC.
%                       The units of the date are days. The full date is
%                       the sum of both terms. The date is broken into two
%                       parts to provide more bits of precision. It does
%                       not matter how the date is split.
%           point       The [x;y;x] Cartesian location under consideration
%                       in 3D ECEF coordinates with units of meters.
%           rhow        An optional parameter specifying the mass density
%                       of water vapor at the point in question in units of
%                       kilograms per cubic meter. If omitted, the air is
%                       assumed to be dry (rhow=0). The total density of
%                       the air is assumed to be the sum of the dry air
%                       density and rhow.
%
%OUTPUTS:   N           The refractivity of the atmosphere. In this model,
%                       N is always real. N=10^6*(n-1) where n is the index
%                       of refraction. This is generally valid for
%                       frequencies from L-band (1GHz) to 10 GHz (the
%                       middle of X-band).
%           rho         The atmospheric density at the point in question in
%                       units of kilograms per cubic meter. 
%           T           The temperature at the point in question with units
%                       of degrees Kelvin.
%           P           The atmospheric pressure at the point in question
%                       in units of Newtons per square meter (Pascals). It
%                       assumes that the gasses can be treated as ideal
%                       gasses.
%
%The dry air density and temperature is obtained from the NRLMSISE-00
%atmospheric model using the default parameters for magnetic and solar
%parameters, which are generally valid below 90km, using the function
%standAtmosGasTemp. The refractive index is then found using the dry and
%wet air densities using the formula of
%J. M. Aparicio and S. Laroche, "An evaluation of the expression of
%the atmospheric refractivity for GPS signals," Journal of Geophysical
%Research, vol. 116, no. D11, 16 Jun. 2011.
%which should be valid for frequencies between 1GHz and 10GHz. It ignores
%the lossiness of the atmosphere.
%
%The atmospheric pressure is obtained using the Ideal Gas Law as described
%in THE NRLMSISE-00 AND HWM-93 USERS GUIDE: Version 1.50, November 2003 by
%Douglas P. Drob.
%
%The anomalous oxygen parameter in the NRLMSISE-00 model is not used.
%
%December 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<4)
   rhow=0; 
end

[~,t,d]=standAtmosGasTemp(Jul1,Jul2,point);

T=t(2);%The temperature in Kelving at the point.
rhod=d(6);%The total mass density of dry air at the point in kg/m^3.
rho=rhod+rhow;

%To use the ideal gas law to find the air pressure, the number of water
%molecules per cubic meter of the atmosphere is needed. This is obtained
%using the molar mass of water (H20) and Avogadro's constant
Av=Constants.AvogadroConstant;
%The molar mass of water (H20) in atomic mass units (grams per mole).
HAMU=Constants.elementAMU(1);
OAMU=Constants.elementAMU(8);
MMWater=HAMU*2+OAMU;

%The number of atoms of water per cubic meter. The 1000 transforms grams to
%kilograms.
NH2O=rhow/(1000*Av*MMWater);

%The total number density of the gasses in the atmosphere. That is, the
%number of atoms per cubic meter.
NTotal=sum(d(1:5))+d(7)+NH2O;
kB=Constants.BoltzmannConstant;
P=NTotal*kB*T;

%T is the temperature in Kelvin.
tau=273.15/T-1;
N0=(222.682+0.069*tau)*rhod+(6701.605+6385.886*tau)*rhow;
N=N0*(1+10^(-6)*N0/6);
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
