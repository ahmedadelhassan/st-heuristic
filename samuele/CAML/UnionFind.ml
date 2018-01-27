(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

type 'a union_find = 'a list list

(**********************************************************************)

let find uf x =
    List.find (fun cl -> (List.exists (fun y -> y = x) cl)) uf

let are_in_same_class uf x_1 x_2 =
    (find uf x_1) = (find uf x_2)

let union uf x_1 x_2 =
    let cl_1 = find uf x_1 and cl_2 = find uf x_2 in
    if cl_1 = cl_2 then
        uf
    else
        let new_uf = 
            List.filter (fun cl -> cl <> cl_1 && cl <> cl_2) uf 
        in
        (List.append cl_1 cl_2) :: new_uf

