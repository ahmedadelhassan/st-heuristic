(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

exception Option_None

let get_option x =
    match x with
        |Some xx -> xx
        |None -> raise Option_None

let is_some x =
    match x with
        |Some _ -> true
        |None -> false

let alternative_option x a b =
    match x with
        |Some xx -> a xx
        |None -> b

let rec filtered_map filter map lst =
    match lst with
        |[] -> []
        |e :: tail when filter e ->
            (map e) :: (filtered_map filter map tail)
        |e :: tail -> filtered_map filter map tail

(** inter a b : returns the list [a ; a + 1 ; ... ; b]. *)
let rec inter a b =
    if a > b then
        []
    else
        a :: (inter (a + 1) b)

(** lists_product lst_1 lst_2 : returns the list of all pairs whose
 *  first coordinate is in lst_1 and the second in lst_2. For instance,
 * # lists_product [1 ; 2 ; 3] ['a' ; 'b'];;
 * - : (int * char) list =
 *   [(1, 'a'); (1, 'b'); (2, 'a'); (2, 'b'); (3, 'a'); (3, 'b')]
**)
let rec lists_product lst_1 lst_2 =
    List.flatten
        (List.map
            (fun e_1 -> List.map (fun e_2 -> (e_1, e_2)) lst_2)
            lst_1)
