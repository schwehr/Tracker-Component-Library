function [q,p,nc]=getNextSetPartition(n,q,p,nc)
%%GETNEXTSETPARTITION Get the next way of partitioning a set of n unique
%                  items. The partition is a length-n vector q that
%                  specifies which partition each item belongs to. The
%                  order of the items in the partitions is not important
%                  and the order of the partitions themselves is not
%                  important. If only n is passed, then the values for the
%                  first partition in the sequence are returned.
%
%INPUTS: n The number of items in the set to be partitioned. If this
%          parameter is passed alone, then the values in the first
%          partition are returned.
%        q The current set partition that should be updated to get the next
%          set partition.
%        p A return value for the current partition; p(i) is the number of
%          elements in the ith class of the output partition for i=1 to nc.
%       nc A return value for the current partition; the number of classes
%          in the output partition (the number of subsets).
%
%OUTPUTS: q The updated partition. If the final partition was passed, then
%           q will be an empty matrix. If only n was passed, then q will be
%           the first partition.
%      p,nc The updated values of p and nc so that the function
%           nextSetPartition can be called again to get the next set
%           partition if q was not empty. If nc==n, then the returned
%           partition is the final partition and a subsequent call will
%           return an empty matrix.
%
%The algorithm is NEXEQU in Chapter 11 of
%A. Nijenhuis and H. S. Wilf, Combinatorial Algorithms for Computers
%and Calculators, 2nd ed. New York: Academic press, 1978.
%
%Given a set (1,2,3), the possible partitions and the corresponding values
%of q are
%(1,2,3)        q=[1;1;1]
%(1,2)(3)       q=[1;1;2]
%(1,3)(2)       q=[1;2;1]
%(2,3)(1)       q=[1;2;2]
%(1)(2)(3)      q=[1;2;3]
%This function always returns q so that the partition numbers increase.
%That is, there are no jumps. For example, q=[2;1;2] is equivalent to
%q=[1;2;1], but this function will only return the latter.
%
%%The function is either called as
%[q,p,nc]=getNextSetPartition(n);
%to get the first set parition or as
%[q,p,nc]=getNextSetPartition(n,q,p,nc);
%to get subsequent set partitions.
%
%October 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

%If only n was passed, get the first set partition.
if(nargin==1)
    p(1)=n;
    nc=1;
    q=ones(n,1);
    return;
end

%If the final set partition was passed, return the empty matrix.
if(nc==n)
   q=[];
   return;
end


%Step B.
m=n;

%Step C
while(1)
    L=q(m);
    if(p(L)~=1)
        break;
    end
    q(m)=1;
    m=m-1;
end

%Step D
nc=nc+m-n;
p(1)=p(1)+n-m;
if(L==nc)
    nc=nc+1;
    p(nc)=0;
end

%Step E
q(m)=L+1;
p(L)=p(L)-1;
p(L+1)=p(L+1)+1;

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
