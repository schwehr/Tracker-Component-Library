function beta=calc2DAssignmentProbsApprox(A,approxType)
%%CALC2DASSIGNMENTPROBSAPPROX Given a matrix of all-positive likelihoods or
%                likelihood ratios, determine the probability that each row
%                (target) is assigned to each column (measurement or missed
%                detection hypothesis). Muultiple approximation algorithms
%                are available. This function is useful when the function
%                calc2DAssignmentProbs is too slow. Missed detection
%                likelihoods are required (but can be zero) and are
%                appended as on the main diagonal of a matrix to the end of
%                A.
%
%INPUTS:    A  A matrix of positive likelihoods or likelihood ratios (NOT
%              log-likelihood ratios). A is a numTar X (numMeas+numTar)
%              matrix of all-positive likelihoods or likelihood ratios for
%              assigning the target specified by the row to the measurement
%              specified by the column. Columns > numMeas hold
%              missed-detection likelihoods. Thus, off-diagonal terms for
%              columns > numMeas should be set to 0 and the diagonal terms
%              set to the costs of a missed detection for each given
%              target.
%   approxType An optional parameter specifying the type of approximation
%              to use. The default if omitted is 0. Possible values are
%              0 The Cheap JPDA of [1].
%              1 The modified cheap JPDAF of [2].
%              2 The algorithm of [3].
%              3 The matrix permanent approximation algorithm of [4].
%
%OUTPUTS: beta  beta is a numTar X (numMeas+1) matrix of probabilities of
%               assigning the target given by the row to the measurement
%               given by the column. The final column is a set of missed
%               detection probabilities. Thus, in this case, beta has fewer
%               columns than A, because the final numRow columns are
%               collapsed into one column on return.
%
%Approximations to target-measurement association hypotheses  are necessary
%when a large number of targets/ measurements are present as the complexity
%scales exponentially with the number of targets. The references below are
%the algorithms; reference [5] compares the algorithms in [1]-[4].
%
%REFERENCES:
%
%[1] R. J. Fitzgerald, "Development of practical PDA logic for multitarget
%    tracking by microprocessor," in Multitarget-Multisensor Tracking:
%    Advanced Applications, Y. Bar-Shalom, Ed. Norwood, MA.: Artech House,
%    1996, pp. 1-23.
%[2] P. Quan, Z. Hongcai, W. Peide and H. Zhou, "Development of new
%    practical multitarget data association algorithm," Proceedings of the
%    First IEEE Conference on Control Applications, vol. 2, Dayton, OH,
%    Sep. 1992, pp. 1065-1066.
%[3] B. Bakhtiar and H. Alavi, "Efficient algorithm for computing data
%    association probabilities for multitarget tracking," Proceedings of
%    SPIE: Automatic Object Recogniation IV, vol. 2756, Apr. 1996, pp.
%    130-140.
%[4] J. K. Uhlmann, "Matrix permanent inequalities for approximating joint
%    assignment matrices in tracking systems," Journal of the Franklin
%    Institute, vol. 341, no. 7, pp. 569-593, Nov. 2004.
%[5] K. Romeo, D. F. Crouse, Y. Bar-Shalom, and P. Willett, "The JPDAF in
%    practical systems: Approximations," in Proceedings of SPIE:
%    Signal and Data Processing of Small Targets Conference, vol. 7698,
%    Apr. 2010.
%
%March 2015 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<2)
    approxType=0;
end

numTar=size(A,1);
numMeas=size(A,2)-numTar;
beta=zeros(numTar,numMeas+1);

switch(approxType)
    case 0%Cheap JPDAF
        for curTar=1:numTar
            Str=sum(A(curTar,1:numMeas));
            %Missed detection likelihood for this target
            B=A(curTar,numMeas+curTar);
            for curMeas=1:numMeas 
                Sji=sum(A(1:numTar,curMeas));

                Gtj=A(curTar,curMeas);

                beta(curTar,curMeas)=Gtj/(Sji+Str-Gtj+B);
            end

            %The missed detection hypothesis
            beta(curTar,numMeas+1)=1-sum(beta(curTar,1:numMeas));
        end
    case 1%Quan, Hongcai, Peide and Zhou?s Algorithm
        %First, find the r's.
        r=zeros(numTar,numMeas);
        for curTar=1:numTar
            Str=sum(A(curTar,1:numMeas));
            for curMeas=1:numMeas
                r(curTar,curMeas)=A(curTar,curMeas)/Str;
            end
        end

        %Next, compute the approximate probabilities.
        for curTar=1:numTar
            sum1=sum(A(curTar,1:numMeas));
            for curMeas=1:numMeas
                sum2=sum(r(:,curMeas).*A(:,curMeas));
                %Missed detection likelihood for this target
                B=A(curTar,numMeas+curTar);
                beta(curTar,curMeas)=A(curTar,curMeas)/(sum1+sum2-r(curTar,curMeas)*A(curTar,curMeas)+B); 
            end
            
            %The missed detection hypothesis
            beta(curTar,numMeas+1)=1-sum(beta(curTar,1:numMeas));
        end
    case 2%The Bakhtiar and Alavi Algorithm
        beta=zeros(numTar,numMeas+1);
        
        %It is simpler just to put the missed detection hypotheses as an
        %extra column at the end.
        GMissedDet=diag(A(:,(numMeas+1):end));
        G=[A(:,1:numMeas),GMissedDet];
        for curTar=1:numTar
           tarSel=[1:(curTar-1),(curTar+1):numTar];

           for curMeas=1:numMeas
               %The +1 selects the missed detection likelihood
               measSel=[1:(curMeas-1),(curMeas+1):(numMeas+1)];
               
               beta(curTar,curMeas)=G(curTar,curMeas)*prod(sum(G(tarSel,measSel),2));
           end
           %The missed detection hypothesis
           beta(curTar,numMeas+1)=G(curTar,curMeas+1)*prod(sum(G(tarSel,:),2));
        end
        
        %Normalize the probabilities.
        beta=bsxfun(@rdivide,beta,sum(beta,2));
    case 3%Uhlmann's algorithm using approximate matrix permanents.
        numTar=size(A,1);
        numCol=size(A,2);
        numMeas=size(A,2)-numTar;

        boolRowsSkip=false(numTar,1);
        boolColsSkip=false(numCol,1);

        beta=zeros(numTar,numMeas+1);
        for curTar=1:numTar
            boolRowsSkip(curTar)=true;

             for curMeas=1:numMeas
                 ati=A(curTar,curMeas);
                 %The A matrix removing the row and column corresponding to the
                 %selected association.
                 boolColsSkip(curMeas)=true;
                 beta(curTar,curMeas)=ati*permBound(A(~boolRowsSkip,~boolColsSkip));

                 boolColsSkip(curMeas)=false;
             end
             %The missed detection hypothesis
             curMeas=numMeas+curTar;
             ati=A(curTar,curMeas);

             boolColsSkip(curMeas)=true;
             beta(curTar,numMeas+1)=ati*permBound(A(~boolRowsSkip,~boolColsSkip));

             boolColsSkip(curMeas)=false;
             boolRowsSkip(curTar)=false;
        end
        
        %Normalize the probabilities.
        beta=bsxfun(@rdivide,beta,sum(beta,2));
    otherwise
        error('Invalid Approximation Specified')
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
