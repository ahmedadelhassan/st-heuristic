(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** filtered_map filter map lst : returns the sublist of lst obtained
 *  by transforming by map its elements satisfying the predicate filter.
**)
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

(** list_suppress_runs lst : returns the list obtained from the list
 *  `lst` obtained by replacing each factor of equal elements by one
 *  single occurrence.
 *  # list_suppress_runs [0; 1; 1; 0; 0; 0; 0; 2; 1; 1];;;
 *  - : int list = [0; 1; 0; 2; 1]
**)
let rec list_suppress_runs lst =
    match lst with
        |[] -> []
        |[x] -> [x]
        |e_1 :: e_2 :: tail when e_1 = e_2 -> 
            list_suppress_runs (e_2 :: tail)
        |e :: tail -> e :: (list_suppress_runs tail)
        

