  
import java.util.*;
/**
 * Define a common behavior for class <code>Node</code> in the graph.
 * A graph is defined as a set of nodes and edges.
 * Nodes are abstract entities  that represent points.
 * Edges are conections between the nodes
 * @author Omar Garcia
 * @version 1.0 date: 12 August 2002
 * @see  Node
 */
public interface NodeIfc { 
    
    /**
	 * Getter for the node's name
	 * @return     the name of the node
	 */
	public String getName();
	
	 /**
	 * Setter for the node's name.
	 * @param  newName   A new name is set for the node
	 */
	public void setName(String newName);
	 
	 /**
	 * List of all nodes that are connected to this one
	 * @return Vector of Nodes 
	 */
	
	public Vector getNextNodes();
	
	 /**
	 * Check if there is a edge between <code> this </code> and thatNode 
	 * @param  node   the checked node
	 * @return     true if there is an edge between <code> this </code> and thatNode
	 */
	public boolean isEdgeTo (Node thatNode);
} 