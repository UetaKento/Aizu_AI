load fisheriris % データをロードする
ctree = fitctree(meas,species); %決定木を設計する。
view(ctree); % 決定木をルールの形で表示する
view(ctree,'mode', 'graph'); % 決定木をグラフの形でで表示する