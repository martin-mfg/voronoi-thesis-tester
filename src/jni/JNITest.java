package jni;

public class JNITest {

	private static native void callnative();

	public static void test() {
		new JNITest().callnative();
	}

	static {
		System.loadLibrary("hello");
	}
}
