/**SPHERHARMONICCOVCPPINT A mex file interface to the C++ implementation of
 *                     the algorithm for computing variances and
 *                     covariances associated with estimates derived by
 *                     synthesizing spherical harmonic coefficients.
 *                     Generally, the Matlab function spherHarmonicEval
 *                     should be called instead of this one, as this
 *                     function does no input checking and running the
 *                     function with invalid inputs will crash Matlab.
 *
 *This function exists, because one can not access the properties of a
 *Matlab class in C++ without Matlab making a copy of the data. However, 
 *when large coefficient sets are used, this wastes memory and is slow.
 *Thus, rather than creating a mex file with the name spherHarmonicCov
 *that will get run in place of the .m file, a condition was inserted
 *into spherHarmonicCov.m to check for the existence of a compiled version
 *of this function and the individual elements of the ClusterSet classes
 *for the coefficients are passed.
 *
 *The algorithm can be compiled for use in Matlab using the 
 *CompileCLibraries function.
 *
 *The function is called in Matlab using the format:
 *[sigma2,Sigma]=spherHarmonicCovCPPInt(CStdDev,SStdDev,offsetArray,clusterSizes,point,a,c,scalFactor);
 *or using 
 *[sigma2]=spherHarmonicCovCPPInt(CStdDev,SStdDev,offsetArray,clusterSizes,point,a,c,scalFactor);
 *if one only the variance of the potential is desired. The function
 *executes faster if only the variance of the potential and not the
 *covariance matrix of the gradient need be computed.
 *
 *April 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
 */
/*(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.*/

#include"matrix.h"
#include "mex.h"
/* This header validates inputs and includes a header needed to handle
 * Matlab matrices.*/
#include "MexValidation.h"
#include "mathFuncs.hpp"

void mexFunction(const int nlhs, mxArray *plhs[], const int nrhs, const mxArray *prhs[]) {
    double a,c,scalFactor;
    double *point;
    ClusterSetCPP<double> CStdDev;
    ClusterSetCPP<double> SStdDev;
    size_t numPoints;
    mxArray *sigma2MATLAB;
    //This variable is only used if nlhs>1. It is set to zero here to
    //suppress a warning if compiled using -Wconditional-uninitialized.
    mxArray *SigmaMATLAB=NULL;
    double *sigma2,*Sigma;
    
    if(nrhs!=8) {
        mexErrMsgTxt("Wrong number of inputs.");
    }
    
    CStdDev.clusterEls=(double*)mxGetData(prhs[0]);
    SStdDev.clusterEls=(double*)mxGetData(prhs[1]);
    
    CStdDev.offsetArray=(size_t*)mxGetData(prhs[2]);
    CStdDev.clusterSizes=(size_t*)mxGetData(prhs[3]);
    SStdDev.offsetArray=CStdDev.offsetArray;
    SStdDev.clusterSizes=CStdDev.clusterSizes;
    
    CStdDev.numClust=mxGetM(prhs[2]);
    SStdDev.numClust=CStdDev.numClust;
    {
        size_t M;
        M=CStdDev.numClust-1;
        CStdDev.totalNumEl=(M+1)*(M+2)/2;
    }
    SStdDev.totalNumEl=CStdDev.totalNumEl;

    //Get the other parameters.
    checkRealDoubleArray(prhs[4]);
    point=(double*)mxGetData(prhs[4]);
    numPoints=mxGetN(prhs[4]);
    a=getDoubleFromMatlab(prhs[5]);
    c=getDoubleFromMatlab(prhs[6]);
    scalFactor=getDoubleFromMatlab(prhs[7]);
    
    //Allocate space for the return values
    sigma2MATLAB=mxCreateDoubleMatrix(numPoints, 1,mxREAL);
    sigma2=(double*)mxGetData(sigma2MATLAB);
    
    if(nlhs>1) {
        mwSize dims[3];
        //A 3X3X numPoints array is needed for the covariance matrices.
        dims[0]=3;
        dims[1]=3;
        dims[2]=numPoints;
        
        SigmaMATLAB=mxCreateNumericArray(3,dims,mxDOUBLE_CLASS,mxREAL);
        Sigma=(double*)mxGetData(SigmaMATLAB);
    } else {
        Sigma=NULL;
    }
    spherHarmonicCovCPP(sigma2,Sigma,CStdDev,SStdDev,point,numPoints,a,c,scalFactor);

    plhs[0]=sigma2MATLAB;
    
    if(nlhs>1) {
        plhs[1]=SigmaMATLAB;
    }
}

/*LICENSE:
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
%OF RECIPIENT IN THE USE OF THE SOFTWARE.*/
