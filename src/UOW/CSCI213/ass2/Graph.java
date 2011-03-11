   
import java.util.*;
/**
  * Graph       A graph is defined like this G = {Nodes, Edges}
  * {@link GraphItf Defined in GraphItf}
  * @version    1.0    Date: 15 August 2002
  * @author     Omar Garcia
**/
public abstract class Graph implements GraphIfc {
    protected String name;
    protected Vector  listNodes;
    
    /**
    * Constructor for the Graph Class
    * @parameter myName The name of the graph
    */
    public Graph(String name) {
        this.name = name;
        listNodes = new Vector();
    }
    
    /**
    *  {@link NodeIfc#addNode Defined in NodeIfc}
    */ 
    public Node addNode (String newName) {
        Node node = new Node(newName);
        listNodes.addElement(node);
        return (node);
    }
    
    /**
     *  {@link NodeIfc#getNodes Defined in NodeIfc}
     */         
    public final Vector getAllNodes() {
        return (listNodes);
    }
    
    /**
     *  {@link NodeIfc#removeNode Defined in NodeIfc}
     */ 
    public boolean removeNode (Node node){
     // remove the node only if it not connected at all
     if (node.getNextNodes().size() > 0) {
         return (listNodes.removeElement(node));
     }   
     else return false;  // has edges connected to i
    }

    /**
     *  {@link NodeIfc#addEdge Defined in NodeIfc}
     */     
    public Edge addEdge (Node n1, Node n2) {
       if (listNodes.contains(n1) & listNodes.contains(n2)) {
            Edge e = new Edge(n1, n2);
            n1.addEdgeTo(n2); // redundancy in order to improve speed
            n2.addEdgeTo(n1); // redundancy in order to improve speed
            return e;
       }
       else return(null);
    }
    
    /**
     *  {@link NodeIfc#removeEdge Defined in NodeIfc}
     */         
    public boolean removeEdge (Edge edge) {
       return (listNodes.removeElement(edge));
    }
    
    /**
     *  Verify if a node with the same name exists
     *  @parameter name The name to look for
     *  @return  the reference to the node that has the same name
     *  Only the first one is returned.
     */      
    public Node nodeNameExist(String name) {
        String nameNode;
        for (int i = 0; i < listNodes.size(); i++) {
            nameNode = ((Node) listNodes.elementAt(i)).getName();
            if ( nameNode.equals(name) ) {
                return (Node) listNodes.elementAt(i);
                }
            }
        return (null);
    }

    /**
    * Load the Road map by using addEdge method  mainly 
    */
    public abstract void loadData();
}
