(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

(** A vertex is a natural integer. *)
type vertex = int

(** An edge is an ordered pair of vertices. *)
type edge = vertex * vertex

(** Representation of a graph for an instance of the Steiner tree 
 *  problem.
 * A graph is here a functional data.
**)
type graph = {
    nb_vertices : int;
    is_terminal : vertex -> bool;
    edge_value : edge -> int option
}

(**********************************************************************)

(** nb_vertices graph : returns the number of vertices of graph. *)
let nb_vertices graph = 
    graph.nb_vertices

(** vertices graph : returns the list [1 ; ... ; n] of all vertices of
 *  graph.
**)
let vertices graph = 
    Tools.inter 1 (nb_vertices graph)

(** terminal_vertices graph : returns the list [1 ; ... ; n] of all 
 *  terminal vertices of graph.
**)
let terminal_vertices graph =
    List.filter (fun x -> graph.is_terminal x) (vertices graph)

(** nb_terminal_vertices graph : returns the number of terminal vertices
 *  of graph.
**)
let nb_terminal_vertices graph =
    List.length (terminal_vertices graph)

(** vertices_pairs graph : returns the list of all pairs of vertices of 
 * graph. 
**)
let vertices_pairs graph =
    let vertices = vertices graph in
    Tools.lists_product vertices vertices 

(** valued_edges graph : returns the list of all triples (x, y, w) where
 *  (x, y) is an edge valued by w in graph.
**)
let valued_edges graph =
    Tools.filtered_map 
        (fun (x, y, v) -> Tools.is_some v)
        (fun (x, y, v) -> (x, y, Tools.get_option v))
        (List.map 
            (fun (x, y) -> (x, y, graph.edge_value (x, y)))
            (vertices_pairs graph))

(** nb_edges graph : returns the number of edges in graph. *)
let nb_edges graph = 
    List.length (valued_edges graph)

(** to_string graph : returns a string for the graph in the format
 *  of PACE 2018 (STP file Format).
**)
let to_string graph =
    let graph_data_string =
        Printf.sprintf "Nodes %d\nEdges %d\n" 
            (nb_vertices graph)
            (nb_edges graph)
    in    
    let edges_values_string =
        List.fold_left (^) "" 
            (List.map
                (fun (x, y, v) -> Printf.sprintf "E %d %d %d\n" x y v)
                (valued_edges graph))
    in
    let terminal_data_string =
        Printf.sprintf "Terminals %d\n" (nb_terminal_vertices graph)
    in
    let terminal_vertices_string = 
        List.fold_left (^) "" 
            (List.map
                (fun x -> Printf.sprintf "T %d\n" x)
                (terminal_vertices graph))
    in
    Printf.sprintf 
        "SECTION Graph\n%s%sEND\nSECTION Terminals\n%s%sEND\n"
        graph_data_string edges_values_string terminal_data_string 
        terminal_vertices_string

(** from_string str : returns a graph specified by the string str
 *  formated following the specifications of PACE 2018 (STP file 
 *  Format).
 *  NOT EFFICIENT.
**)
let from_string str =

    let preprocess = Str.split (Str.regexp "[ \t\n]+") str in 
    let stream = Scanf.Scanning.from_string (String.concat " " preprocess) in
    let _ = Scanf.bscanf stream "SECTION Graph%s " (fun s -> None) in
    let nb_vertices = Scanf.bscanf stream "Nodes %d " (fun n -> n) in
    let nb_edges = Scanf.bscanf stream "Edges %d " (fun n -> n) in
    let rec parse_edges res =
        try 
            parse_edges 
            (Scanf.bscanf stream "E %d %d %d " 
                (fun x y w -> (x, y, w) :: res))
        with
            |Scanf.Scan_failure _ -> res
            |End_of_file -> res
    in
    let valued_edges = parse_edges [] in
    let _ = Scanf.bscanf stream "ND%s " (fun s -> None) in
    let _ = Scanf.bscanf stream "SECTION Terminals%s " (fun s -> None) in
    let nb_terminals = Scanf.bscanf stream "Terminals %d " 
        (fun n -> n) 
    in
    let rec parse_terminals res =
        try 
            parse_terminals
            (Scanf.bscanf stream "T %d " 
                (fun x -> x :: res))
        with
            |Scanf.Scan_failure _ -> res
            |End_of_file -> res
    in
    let terminals = parse_terminals [] in
    let _ = Scanf.bscanf stream "END%s " (fun s -> None) in
    let is_terminal x = 
        List.mem x terminals 
    in
    let edge_value edge =
        let (x, y) = edge in
        let rec find lst =
            match lst with
                |[] -> None
                |(xx, yy, ww) :: tail when xx = x && yy = y -> Some ww
                |_ :: tail -> find tail
        in
        find valued_edges
    in
    {
        nb_vertices = nb_vertices ;
        is_terminal = is_terminal ;
        edge_value = edge_value
    }

