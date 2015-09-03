function q=unrankComposition(rank,n,m)
%%UNRANKCOMPOSITION Return the composition of the given rank, starting from
%                   zero, of n unlabeled items in m parts. That is, a
%                   method of putting n unlabeled balls into m labeled
%                   slots.
%
%INPUTS:     rank   The rank (position in an ordered sequence) of the
%                   desired composition starting from zero. There is a
%                   total of binomial(n+m-1,m-1) compositions.
%            n      The number of items that are composed into slots, >=1.
%            m      The number of slots that can hold items, >=1.
%
%OUTPUTS:    q      An mX1 vector holding the current composition, whose
%                   elements sum to n. Each element is the number of
%                   "balls" in that slot. If rank is larger than the
%                   maximum number of compositions, an empty matrix is
%                   returned. The values of q can range from 0 to n.
%
%This is an implementation of the relation described in Chapter 7.2.1.3 of
%D. E. Knuth, The Art of Computer Programming. Vol. 4, Fascicle 3:
%Generating all Combinations and Partitions, Upper Saddle River, NJ:
%Addison-Wesley, 2009.
%for mapping combinations to compositions. rank is used to obtain a
%combination in Lexicographic order and the elements of the combination are
%mapped to the equivalent composition.
%
%October 2013 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

nCombo=n+m-1;
mCombo=m-1;

if(mCombo>0)
    c=unrankCombination(rank,nCombo,mCombo);

    if(isempty(c))
        q=[];
        return;
    end

    d=c-(1:mCombo)'+1;

    %Transform the combination into a valid composition.
    q=zeros(m,1);
    q(1)=d(1);
    for curIdx=2:(m-1)
        q(curIdx)=d(curIdx)-d(curIdx-1);
    end
    q(m)=n-d(mCombo);
else%The m=1 case has to be handled separately.
    
    if(rank==0)
        q=n;
    else
       q=[]; 
    end
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
