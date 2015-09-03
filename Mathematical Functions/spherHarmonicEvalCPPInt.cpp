/**SPHERHARMONICEVALCPPINT A mex file interface to the C++ implementation
 *                     of the spherical Harmonic synthesis algorithm.
 *                     Generally, the Matlab function spherHarmonicEval
 *                     should be called instead of this one, as this
 *                     function does no input checking and running the
 *                     function with invalid inputs will crash Matlab.
 *
 *This function exists, because one can not access the properties of a
 *Matlab class in C++ without Matlab making a copy of the data. However, 
 *when large coefficient sets are used, this wastes memory and is slow.
 *Thus, rather than creating a mex file with the name spherHarmonicEval
 *that will get run in place of the .m file, a condition was inserted
 *into spherHarmonicEval.m to check for the existence of a compiled version
 *of this function and the individual elements of the ClusterSet classes
 *for the coefficients are passed.
 *
 *The algorithm can be compiled for use in Matlab using the 
 *CompileCLibraries function.
 *
 *The function is called in Matlab using the format:
 *[V,gradV]=spherHarmonicEvalCPPInt(CCoeffs,SCoeffs,offsetArray,clusterSizes,point,a,c,scalFactor);
 *or using 
 *[V]=spherHarmonicEvalCPPInt(CCoeffs,SCoeffs,offsetArray,clusterSizes,point,a,c,scalFactor);
 *if one only wants the potential. The function executes faster if only the
 *potential and not the gradient need be computed.
 *
 *January 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
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
    ClusterSetCPP<double> C;
    ClusterSetCPP<double> S;
    size_t numPoints;
    mxArray *VMATLAB;
    //This variable is only used if nlhs>1. It is set to zero here to
    //suppress a warning if compiled using -Wconditional-uninitialized.
    mxArray *gradVMATLAB=NULL;
    double *V,*gradV;
    
    if(nrhs!=8) {
        mexErrMsgTxt("Wrong number of inputs.");
    }
    
    C.clusterEls=(double*)mxGetData(prhs[0]);
    S.clusterEls=(double*)mxGetData(prhs[1]);
    
    C.offsetArray=(size_t*)mxGetData(prhs[2]);
    C.clusterSizes=(size_t*)mxGetData(prhs[3]);
    S.offsetArray=C.offsetArray;
    S.clusterSizes=C.clusterSizes;
    
    C.numClust=mxGetM(prhs[2]);
    S.numClust=C.numClust;
    {
        size_t M;
        M=C.numClust-1;
        C.totalNumEl=(M+1)*(M+2)/2;
    }
    S.totalNumEl=C.totalNumEl;

    //Get the other parameters.
    checkRealDoubleArray(prhs[4]);
    point=(double*)mxGetData(prhs[4]);
    numPoints=mxGetN(prhs[4]);
    a=getDoubleFromMatlab(prhs[5]);
    c=getDoubleFromMatlab(prhs[6]);
    scalFactor=getDoubleFromMatlab(prhs[7]);
    
    //Allocate space for the return values
    VMATLAB=mxCreateDoubleMatrix(numPoints, 1,mxREAL);
    V=(double*)mxGetData(VMATLAB);
    
    if(nlhs>1) {
        gradVMATLAB=mxCreateDoubleMatrix(3, numPoints,mxREAL);
        gradV=(double*)mxGetData(gradVMATLAB);
    } else {
        gradV=NULL;
    }
    spherHarmonicEvalCPP(V, gradV,C,S,point,numPoints,a,c,scalFactor);

    plhs[0]=VMATLAB;
    
    if(nlhs>1) {
        plhs[1]=gradVMATLAB;
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
