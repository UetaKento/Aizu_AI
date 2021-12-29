parent(sofu,otosan).

parent(msofu,okasan).

parent(otosan,miho).

parent(otosan,taro).

parent(otosan,jiro).

parent(X,Y):- married(Z,X),parent(Z,Y).

married(otosan,okasan).

married(msofu,msobo).

married(sofu,sobo).

ancestor(X,Y):-parent(X,Y).

ancestor(X,Y):-parent(X,Z),ancestor(Z,Y).
