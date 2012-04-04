import java.math.BigInteger;

public class rsa{
    public static void main(String[] args){
	String s = "Give me an A";
	String a = "";
	for ( int i = 0; i < s.length(); ++i ) {
	    char c = s.charAt( i );
	    int j = (int) c;
	    a += j;
	}
	System.out.println(a);

  BigInteger n = new BigInteger(a);

	a =	n.toString(2);

	System.out.println(a);

	BigInteger key1 = new BigInteger ("46947848749720430529628739081");
	BigInteger key2 = new BigInteger ("37267486263679235062064536973");
	BigInteger x = new BigInteger(a);

	x = x.modPow(key2,key1);

	System.out.println(x);
    }
}
