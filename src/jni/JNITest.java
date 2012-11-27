package jni;

public class JNITest {

	private static native void callnative();

	public static void test() {
		new JNITest().callnative();
	}

	static {
		if ((System.getProperty("os.name")=="Linux"))
			System.loadLibrary("hello32.so");
		else
			System.load("/opt/hello32.so");
    	
	}
}
