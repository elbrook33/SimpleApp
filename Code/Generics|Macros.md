.add{NAME}: Dict [dict*], Item [{TYPE}] -> Success [bool]
==========
return:
+ Dict != NULL and *Dict != NULL
+ Item != NULL
+ Item.type == {FLAG}
+ Dict.addItem (Elem)

define:
------ ------ --------
{NAME} {TYPE} {FLAG}
------ ------ --------
Num    num    NumType 
Text   text   TextType
Dict   dict   DictType
------ ------ --------

