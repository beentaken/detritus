  
import java.util.*;
/**
 * Represent a node in a mathematical graph.
 * 
 * @author Omar Garcia
 * @version 1.0   Date:12 August 2002
 * @see NodeIfc
 */
public class Node implements NodeIfc
{
	// name: name of the node
	private String name;
	
	//Vector holding the nodes that are connected to this one
	private Vector nextNode;

	/**
	 * Node Constructor
	 * @param myName Node Given name
	 */
	public Node(String myName)
	{
		// initialise instance variables
		name = myName;
		nextNode = new Vector();
	}

	/**
	 * {@link NodeIfc#getName() Defined in NodeIfc }
	 */
	public String getName()
	{
		return name;
	}
	
	/**
	 * {@link NodeIfc#setName(String newName) Defined in NodeIfc }
	 */
	public void setName(String newName)
	{
		name=newName;
	}
	
	/**
	 * {@link NodeIfc#getNextNodes() Defined in NodeIfc }
	 */
	public Vector getNextNodes()
	{
		return (nextNode);
	}
	
	/**
     * {@link NodeIfc#isEdgeTo(Node myNode) Defined in NodeIfc }
	 */
	public boolean isEdgeTo(Node myNode)
	{
		return (true);
	}
	
	/**
     * Add myNode to the neighbours' list of this Node
	 */
	public void addEdgeTo(Node myNode)
	{
		nextNode.addElement(myNode);
	}
}
