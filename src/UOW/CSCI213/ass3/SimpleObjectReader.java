import java.io.*;
/**
 * SimpleObjectReader is a small class to wrap around the usual ObjectStream
 * to shield you from the exception handling which we haven't yet gotten
 * to in class.
 * <P>It has just three methods of note: one to open a new file for reading,
 * one to read an object from an open file, and one to close the file when done.
 *
 * <P>Any object that you attempt to read must properly implement the Serializable
 * interface. Here is a simple example that shows using the SimpleFileReader to
 * to rehydrate objects from a file and print them:
 * <PRE>
 *    SimpleObjectReader reader = SimpleObjectReader.openFileForReading("shapes");
 *    if (reader == null) {
 *        System.out.println("Couldn't open file!");
 *        return;
 *    }
 *    Object obj;
 *    while ((obj = reader.readObject()) != null)
 *        System.out.println(obj);
 *    reader.close();
 * </PRE>
 * <P>You are free to edit or extend this class, but we don't expect that
 * you should need to make any changes.
 *
 * @version      1.1 15/04/01
 * @author       Julie Zelenski
 */
public class SimpleObjectReader {

    private ObjectInputStream ois;

    /**
     * Opens a new file for reading. The filename can either be a relative
     * path, which will be relative to the working directory of the program
     * when started, or an absolute path. If the file exists and can be
     * opened, a new SimpleObjectReader is returned. If the file cannot be
     * opened (for any reason: wrong name, wrong path, lack of permissions, etc.)
     * null is returned.
     */

    public static SimpleObjectReader openFileForReading(String filename){
    	try {
	    return new SimpleObjectReader(new ObjectInputStream(new FileInputStream(filename)));
    	} catch(IOException e) {
	    return null;
	}
    }

    /**
     * Reads a single object from the file and returns it. If there are
     * no more objects in the file (i.e, we have reached the end of file),
     * null is returned null is returned. null is also
     * returned on any I/O error.
     */

    public Object readObject (){
	try {
	    return ois.readObject();
    	} catch (IOException e) {
	    e.printStackTrace();
	    return null;
	} catch (ClassNotFoundException e) {
	    e.printStackTrace();
	    return null;
	}
    }

    /**
     * Closes the file when done reading.  You should close a reader when
     * you are finished to release the OS resources for use by others.
     */

    public void close (){
      	try {
	    ois.close();
      	}
      	catch (IOException e) {}
    }

    /**
     * Constructor is private so that only means to create a new reader
     * is through the static method which does error checking.
     */

    private SimpleObjectReader(ObjectInputStream ois){
	this.ois = ois;
    }
}
