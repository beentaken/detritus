import java.io.*;

/**
 * SimpleObjectWriter is a small class to wrap around the usual
 * ObjectOutputStream to shield you from the exception handling
 * which we haven't yet gotten to in class.
 * <P>It has just three methods of note: one to open a new file for writing,
 * one to write an object to the file, and one to close the
 * the file when done.
 * <P>Here is a simple example that shows using the SimpleObjectWriter
 * to create a new file and write some objects into it:
 * <PRE>
 *    SimpleObjectWriter writer =
 *               SimpleObjectWriter.openFileForWriting("objects");
 *    if (writer == null) {
 *       System.out.println("Couldn't open file!");
 *       return;
 *    }
 *    writer.writeObject("Here is a string");
 *    writer.writeObject("And another one.");
 *    writer.writeObject(new Date());
 *    writer.close();
 * </PRE>
 * <P>You are free to edit or extend this class, but we don't expect that
 * you should need to make any changes.
 *
 * @version      1.1 15/04/01
 * @author       Julie Zelenski
 */

public class SimpleObjectWriter {

    private ObjectOutputStream oos;

   /**
     * Opens a new file for writing. The filename can either be a relative
     * path, which will be relative to the working directory of the program
     * when started, or an absolute path. If the file can be created, a
     * new SimpleObjectWriter is returned. If the file already exists, this
     * will overwrite its contents. If the file cannot be opened (for any
     * reason: wrong name, wrong path, lack of permissions, etc.),
     * null is returned
     */

    public static SimpleObjectWriter openFileForWriting (String filename){
        try {
	    return new SimpleObjectWriter(new ObjectOutputStream(new FileOutputStream(filename)));
    	}
    	catch (IOException e) {
	    return null;
    	}
    }

    /**
     * Writes a single object to the file. The object must properly implement
     * the Serializable interface.
     */

    public void writeObject (Object o){
    	try {
	    oos.writeObject (o);
    	}
    	catch (IOException e) {
	    e.printStackTrace();
    	}
    }

    /**
     * Closes the file when done writing.  You should close a writer when
     * you are finished to flush its contents to disk and release the OS
     * resources for use by others.
     */

    public void close () {
    	try {
	    oos.writeObject(null); // put null object at end as marker
	    oos.close();
    	}
    	catch (IOException e) {}
    }

    /**
     * Constructor is private so that only means to create a new writer
     * is through the static method which does error checking.
     */

    private SimpleObjectWriter (ObjectOutputStream oos) {
    	this.oos = oos;
    }
}
