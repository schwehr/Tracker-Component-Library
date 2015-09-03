function [xEst,PEst,logLikes]=singleScanUpdate(xHyp,PHyp,A,algSel1,algSel2)
%%SINGLESCANUPDATE Perform the measurement update step in a single-scan
%                  tracking algorithm that uses Gaussian approximations 
%                  to represent the target state before and after the
%                  measurement update given a matrix of likelihoods and a
%                  set of track update hypotheses. The algorithm can be a
%                  global nearest neighbor (GNN) update, a joint
%                  probabilistic data association (JPDA) update, a JPDA*
%                  update, a GNN-JPDA update, or approximate JPDA and
%                  GNN-JPDA updates as well as a naïve nearest neighbor
%                  algorithm. In all instances except for the naïve nearest
%                  neighbor algorithm, it is assumed that each measurement
%                  can be assigned to at most one target, and each target
%                  can be assigned to at most one measurement.
%
%INPUTS: xHyp  An xDimXnumTarXnumHyp set of track states updated with each
%              of the (numHyp-1) measurements for each of the numTar
%              targets, with the last hypothesis being the update for the
%              missed detection hypothesis. The order of the measurement
%              hypotheses is the same as the ordering of the likelihoods in
%              the columns of the A matrix.
%        PHyp  An xDimXxDimXnumTarXnumHyp set of covariance matrices for
%              each of the track states for each of the targets updated
%              conditioned on each of the numHyp measurements with the last
%              one being for the missed detection hypothesis.
%           A  A matrix of positive likelihoods or likelihood ratios (NOT
%              log-likelihood ratios). A is a numTar X (numMeas+numTar)
%              matrix of all-positive likelihoods or likelihood ratios for
%              assigning the target specified by the row to the measurement
%              specified by the column. Columns > numMeas hold
%              missed-detection likelihoods. Thus, off-diagonal terms for
%              columns > numMeas should be set to 0 and the diagonal terms
%              set to the costs of a missed detection for each given
%              target. 
%      algSel1 An optional parameter that selects the assignment algorithm
%              to use. Approximate variants of the algorithms also use
%              algSel2. If this parameter and the next parameter are both
%              omitted, then the algorithm is chosen as described below.
%              Possible values are
%              0) GNN-JPDA
%              1) JPDA
%              2) GNN
%              3) Parallel single-target PDAs
%              4) Naïve nearest neighbor
%              5) JPDA*
%              6) Approximate GNN-JPDA
%              7) Approximate JPDA
%              
%      algSel2 An optional parameter that further specifies the algorithm
%              used when algSel1=6-7. If omitted by algSel1 is specified, a
%              default value of 0 is used. If both algSel1 and algSel2 are
%              omitted, the algorithm is chosen as described below.
%              algSel2 chooses the approximation for the beta terms in the
%              function calc2DAssignmentProbsApprox; it corresponds to the
%              approxType parameter in that function and the comments to
%              that function describe the valuesit can take.
%
%OUTPUTS: xEst An xDimXnumTar matrix of updated target states.
%         PEst An xDimXxDimXnumtar matrix of updated covariance matrices
%              for the targets.
%      logLike The numTarX1 set of logarithms of the likelihood (ratio)
%              function for the update for each target. If A contains
%              likelihoods, then this is the logarithm of the likelihood;
%              if A constains likelihood ratios, then this is a logarithm
%              of a likelihood ratio, For hard assignments, this is clearly
%              defined (it is just the entry in A assigned to that target).
%              For soft assignments in the JPDA, JPDA*, PDA, and
%              approximate JPDA this is the expected value of the
%              log-likelihood computed as using the beta terms.
%
%If both algSel1 and algSel2 are omitted, then the algorithm chosen depends
%on the size of A. If A has only one row (one target), then the JPDA is
%used. Otherwise, the approximate GNN-JPDA with algSel2=3 is used (which is
%actually exact if the number of targets is two).
%
%The JPDA is discussed in Chapter 6.2 of
%Y. Bar-Shalom, P. K. Willett, and X. Tian, Tracking and Data Fusion.
%Storrs, CT: YBS Publishing, 2011.
%and the PDA is discussed in various other sections as the JPDA is a
%generalization of the PDA. The JPDA* is discussed in
%H. A. Blom and E. A. Bloem, "Probabilistic data association avoiding track
%coalescence," IEEE Transactions on Automatic Control, vol. 45, no. 2, pp.
%247-259, Feb. 2000.
%The concept fo the GNN-JPDA is described (but not named) in
%O. E. Drummond, "Best hypothesis target tracking and sensor fusion," in
%Proceedings of SPIE: Signal and Data Processing of Small Targets
%Conference, vol. 3809, Denver, CO, Oct. 1999, pp. 586-600.
%The GNN-JPDA consists of using the global nearest neighbor estimate with a
%covariance matrix computed as in the JPDA. Thus the same approximations
%that can be used int eh JPDA can be used in the GNN-JPDA.
%
%The approximate routines simply change how the target-measurement
%association probabilities are computed. They are described in the comments
%to the function calc2DAssignmentProbsApprox.
%
%Naïve nearest neighbor association consists of just choosing the most
%likely assignment of a target to a measurement or missed detection without
%regard to whether one measurmeent is assigned to multiple targets.
%
%One possible way to form the A matrix is to use likelihood ratios going
%into the dimensionless score function from
%Y. Bar-Shalom, S. S. Blackman, and R. J. Fitzgerald, "Dimensionless score
%function for multiple hypothesis tracking," IEEE Transactions on Aerospace
%and Electronic Systems, vol. 43, no. 1, pp. 392-400, Jan. 2007.
%
%March 2015 David Crouse, generalizing the basic JPDAF code of David
%Karnick, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

    numTar=size(A,1);
    numMeas=size(A,2)-numTar;
    numHyp=numMeas+1;
    
    if(nargin<4)
        if(numTar==1)
            algSel1=1;
        else
            algSel1=6;
            algSel2=3;
        end
    end

    if(nargin==4)%If only algSel2 was omitted.
        algSel2=0;
    end
    xDim=size(xHyp,1);
    
    xEst=zeros(xDim,numTar);
    PEst=zeros(xDim,xDim,numTar);
    switch(algSel1)
        case 0%GNN-JPDA
            %Perform 2D assignment
            ALog=log(A);
            tar2Meas=assign2D(ALog,true);
            logLikes=zeros(numTar,1);
            for curTar=1:numTar
                logLikes(curTar)=ALog(curTar,tar2Meas(curTar));
            end

            %Adjust for the index of the missed detection hypothesis.
            sel=tar2Meas>numMeas;
            tar2Meas(sel)=numMeas+1;
            
            %Set the target estimate to the ML estimate.
            for curTar=1:numTar
                maxIdx=tar2Meas(curTar);
                xEst(:,curTar)=xHyp(:,curTar,maxIdx);
            end
            
            %Compute the JPDAF probabilities.
            beta=calc2DAssignmentProbs(A,true);
            
            %Compute the covariance matrix in the manner of the JPDAF, but
            %about the ML estimate, so the result is a MSE matrix estimate.
            for curTar=1:numTar
                x=reshape(xHyp(:,curTar,:),[xDim,numHyp,1]);
                P=reshape(PHyp(:,:,curTar,:),[xDim,xDim,numHyp]);
                [~,PEst(:,:,curTar)]=calcMixtureMoments(x,beta(curTar,:),P,xEst(:,curTar));
            end
        case 1%JPDA
            %Compute the JPDAF probabilities.
            beta=calc2DAssignmentProbs(A,true);
            
            %The JPDAF is just the mixture mean and covariance matrix.
            for curTar=1:numTar
                x=reshape(xHyp(:,curTar,:),[xDim,numHyp,1]);
                P=reshape(PHyp(:,:,curTar,:),[xDim,xDim,numHyp]);
                [xEst(:,curTar),PEst(:,:,curTar)]=calcMixtureMoments(x,beta(curTar,:),P);
            end
            
            %If a likelihood is desired, then find the weighted likelihood.
            if(nargout>2)
                logLikes=logLikefromABeta(A,beta);
            end
        case 2%GNN
            %Perform 2D assignment
            ALog=log(A);
            tar2Meas=assign2D(ALog,true);
            logLikes=zeros(numTar,1);
            for curTar=1:numTar
                logLikes(curTar)=ALog(curTar,tar2Meas(curTar));
            end
            %Adjust for the index of the missed detection hypothesis.
            sel=tar2Meas>numMeas;
            tar2Meas(sel)=numMeas+1;

            for curTar=1:numTar
                maxIdx=tar2Meas(curTar);
                xEst(:,curTar)=xHyp(:,curTar,maxIdx);
                PEst(:,:,curTar)=PHyp(:,:,curTar,maxIdx);
            end
        case 3%Paralle single-target PDAs
            %This is just a bunch of independent PDAFs for each target.
            beta=zeros(numtar,numHyp);
            for curTar=1:numTar
                hypIdx=[1:numMeas,numMeas+curTar];
                beta(curTar,:)=calc2DAssignmentProbs(A(curTar,hypIdx),true);
                x=reshape(xHyp(:,curTar,:),[xDim,numHyp,1]);
                P=reshape(PHyp(:,:,curTar,:),[xDim,xDim,numHyp]);
                [xEst(:,curTar),PEst(:,:,curTar)]=calcMixtureMoments(x,beta(curTar,:),P);
            end
            
            %If a likelihood is desired, then find the weighted likelihood.
            if(nargout>2)
                logLikes=logLikefromABeta(A,beta);
            end
        case 4%Naïve nearest neighbor
            %The assignment for each target
            logLikes=zeros(numTar,1);
            for curTar=1:numTar
                [maxVal,maxIdx]=max(A(curTar,:));
                %If the missed detection hypothesis is the most likely.
                if(maxIdx>numTar)
                    maxIdx=numTar+1;
                end
                
                xEst(:,curTar)=xHyp(:,curTar,maxIdx);
                PEst(:,:,curTar)=PHyp(:,:,curTar,maxIdx);
                logLikes(curTar)=log(maxVal);
            end
        case 5%JPDA*
            %This is the same as the JPDA, except the method for computing
            %the betas is different.
            %Compute the JPDAF* probabilities.
            beta=calcStarBetasBF(A);
            
            %The JPDAF* is just the mixture mean and covariance matrix.
            for curTar=1:numTar
                x=reshape(xHyp(:,curTar,:),[xDim,numHyp,1]);
                P=reshape(PHyp(:,:,curTar,:),[xDim,xDim,numHyp]);
                [xEst(:,curTar),PEst(:,:,curTar)]=calcMixtureMoments(x,beta(curTar,:),P);
            end
            
            %If a likelihood is desired, then find the weighted likelihood.
            if(nargout>2)
                logLikes=logLikefromABeta(A,beta);
            end
        case 6%Approximate GNN-JPDA
            %This is the same as the GNN-JPDA, except the method for
            %computing the betas is different.
            
            %Perform 2D assignment
            ALog=log(A);
            tar2Meas=assign2D(ALog,true);
            logLikes=zeros(numTar,1);
            for curTar=1:numTar
                logLikes(curTar)=ALog(curTar,tar2Meas(curTar));
            end
            
            %Adjust for the index of the missed detection hypothesis.
            sel=tar2Meas>numMeas;
            tar2Meas(sel)=numMeas+1;
            
            %Set the target estimate to the ML estimate
            for curTar=1:numTar
                maxIdx=tar2Meas(curTar);
                xEst(:,curTar)=xHyp(:,curTar,maxIdx);
            end
            
            %Compute the approximate JPDAF probabilities.
            beta=calc2DAssignmentProbsApprox(A,algSel2);
            
            %Compute the covariance matrix in the manner of the JPDAF, but
            %about the ML estimate, so the result is a MSE matrix estimate.
            for curTar=1:numTar
                x=reshape(xHyp(:,curTar,:),[xDim,numHyp,1]);
                P=reshape(PHyp(:,:,curTar,:),[xDim,xDim,numHyp]);
                [~,PEst(:,:,curTar)]=calcMixtureMoments(x,beta(curTar,:),P,xEst(:,curTar));
            end
        case 7%Approximate JPDA
            %This is the same as the JPDA, except the method for computing
            %the betas is different.
            
            %Compute the approximate JPDAF probabilities.
            beta=calc2DAssignmentProbsApprox(A,algSel2);
            
            %The JPDAF* is just the mixture mean and covariance matrix.
            for curTar=1:numTar
                x=reshape(xHyp(:,curTar,:),[xDim,numHyp,1]);
                P=reshape(PHyp(:,:,curTar,:),[xDim,xDim,numHyp]);
                [xEst(:,curTar),PEst(:,:,curTar)]=calcMixtureMoments(x,beta(curTar,:),P);
            end
            
            %If a likelihood is desired, then find the weighted likelihood.
            if(nargout>2)
                logLikes=logLikefromABeta(A,beta);
            end
        otherwise
            error('Unknown algorithm Selected')
    end
end

function logLike=logLikefromABeta(A,beta)
    numTar=size(A,1);
    numHyp=size(beta,2);

    %Create a copy of A with the missed detection likelihoods
    %in the last column and  take the logarithm of it.
    ANew=zeros(numTar,numHyp);
    ANew(:,1:(numHyp-1))=A(:,1:(numHyp-1));
    ANew(:,numHyp)=diag(A(:,numHyp:(numHyp+numTar-1)));
    ANew=log(ANew);

    %Multiply each log-likelihood by the probability of it
    %being correct.
    ANew=ANew.*beta;
    %Get rid of non-finite terms, which should generally only
    %arise in things with zero likelihood.
    sel=~isfinite(ANew);
    ANew(sel)=0;
    %The likelihood is the sum of all of the weighted
    %log-likelihoods.
    logLike=sum(ANew,2);
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
