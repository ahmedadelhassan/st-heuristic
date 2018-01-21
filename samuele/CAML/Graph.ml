(**
 * Author: Samuele Giraudo
 * Creation: jan. 2018
 * Modifications: jan. 2018
**)

type vertex = int

type edge = vertex * vertex

type graph = {
    nb_vertices : int;
    is_terminal : vertex -> bool;
    edge_value : edge -> int option
}

let vertices graph = 
    Tools.inter 1 graph.nb_vertices

let terminal_vertices graph =
    List.filter (fun x -> graph.is_terminal x) (vertices graph)

let nb_terminal_vertices graph =
    List.length (terminal_vertices graph)

let vertices_pairs graph =
    let vertices = vertices graph in
    Tools.lists_product vertices vertices 

let valued_edges graph =
    Tools.filtered_map 
        (fun (x, y, v) -> Tools.is_some v)
        (fun (x, y, v) -> (x, y, Tools.get_option v))
        (List.map 
            (fun (x, y) -> (x, y, graph.edge_value (x, y)))
            (vertices_pairs graph))
    
let nb_edges graph = 
    List.length (valued_edges graph)

let to_string graph =
    let graph_data_string =
        Printf.sprintf "Nodes %d\nEdges %d\n" 
            graph.nb_vertices 
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
        "Section Graph\n%s%sEND\nSECTION Terminals\n%s%sEND\n"
        graph_data_string edges_values_string terminal_data_string 
        terminal_vertices_string

