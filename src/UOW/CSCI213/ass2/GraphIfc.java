   
import java.util.*;
/**
 * GraphIfc	Interface for the graph 
 * @version	1.0  Date: 15 August 2002
 * @author 	Omar Garcia
 */
public interface GraphIfc {
    
    /**
	 * Add a node to the graph. It does not check it he name already exists
	 * @parameter name Given name for the node
	 * @return <code>node</code> 
	 */
	public Node addNode (String name);
	
    /**
	 * Remove a node from the graph. Only remove if there are not edges connected to it
	 *@parameter node To be removed
	 *@return <code> true</code> It it has been sucessfully remove
	 */
	public boolean removeNode (Node n);
	
    /**
	 * Get all nodes that are in the graph.
	 * @return Vector of Nodes
	 */
	public Vector getAllNodes();
	
	/**
	 * Add an Edge to the Graph. Add the edge to the graph only if the two nodes already exists
     * if the edge cannot be added return null.
	 * @parameter n1 first node
	 * @parameter n2 second node
	 * @return the edge
	 */
	public Edge addEdge (Node n1, Node n2);
	
	/**
	 * Remove and Edge form the Graph
	 * @parameter edge  
	 * @return <code> true</code> if the edge was removed
	 */	
	public boolean removeEdge (Edge edge);
}