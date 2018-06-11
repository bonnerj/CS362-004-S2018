import junit.framework.TestCase;
import java.util.*;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!

public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   public void testManualTest()
   {
	   UrlValidator urlValidator = new UrlValidator();
	   HashMap< String, Boolean > test1 = new HashMap < String, Boolean >();
	   test1.put("www.google.com", false);
	   test1.put("google.com", false);
	   test1.put("http://www.google.com/", true); 
	   test1.put("ftp://www.google.com", true); 
	   test1.put("http://google.com", true);
	   test1.put("ftp://google.com", true);
	   test1.put("bad://google.com", false);
	   test1.put(".com", false);
	   test1.put("http://www.google.com:1234", true);
	   test1.put("http://google.com:1234", true);
	   test1.put("http://www.google.com/path", true);
	   test1.put( "http://www.google.com//path", false);
	   test1.put("http://www.google.com/path/", false);
	   test1.put("http://www.google.com/path/query=yes", false);
	   test1.put("http://google.com/path?query=yes", true);
	   test1.put("http://google.com/path?&", false);
	   test1.put("http://www.google.com:1234/path?query=yes", true);
	   test1.put("http://www.google.com/path?query=yes&.", false);
	   test1.put("/noScheme/blank space/", false);
	   
		
       System.out.println("Manual Testing...");
	   for (Map.Entry<String, Boolean> entry: test1.entrySet()) {
		   if (urlValidator.isValid(entry.getKey()) == entry.getValue()) {
			   if (entry.getValue() == true){
				   System.out.println(entry.getKey() + " is valid as expected");
			   }else {
				   System.out.println(entry.getKey() + " is invalid as expected");
			   }
		   } else {
			   if (entry.getValue() == true){
				   System.out.println(entry.getKey() + " failed test as invalid");
			   }else {
				   System.out.println(entry.getKey() + " failed test as valid");
			   } 
		   }
	   }   

       // pass null to function
	   String nullStr = new String();
	   nullStr = null;
	   if (urlValidator.isValid(nullStr)) {
		   System.out.println(nullStr + " failed test as valid");
	   } else {
		   System.out.println(nullStr + " is invalid as expected"); 
	   }
   }
   
//You can use this function to implement your First Partition testing
   public void testYourFirstPartition()
   {
       UrlValidator urlVal = new UrlValidator();

       String partition1[] = {
    		   //Ports 1-65535 are available for use
    	       //Tests boundaries of valid minimum and maximum ports
    		   "http://www.google.com:1", 
    		   "http://www.google.com:65535", 
    		   
    		   //URL Authorities may contain an IP address
    	       //Minimum IP address = 0.0.0.0 and Maximum IP address = 255.255.255.255
    	       //Tests boundaries of valid minimum and maximum IP's
    		   "http://0.0.0.0", 
    		   "http://255.255.255.255"
       };
       
       for (String url : partition1)
    	   if(urlVal.isValid(url)) {
    		   System.out.println(url + " is valid as expected");
    	   } else {
    		   System.out.println(url + " failed test as invalid");
    	   }

   }
   
//You can use this function to implement your Second Partition testing	   
   public void testYourSecondPartition()
   {
       UrlValidator urlVal = new UrlValidator();

       String partition2[] = {
    		   //Ports 1-65535 are available for use
    	       //Tests beyond minimum and maximum ports - invalid
    		   "http://www.google.com:-1", 
    		   "http://www.google.com:65536",
    		   
    		   //URL Authorities may contain an IP address
    	       //Minimum IP address = 0.0.0.0 and Maximum IP address = 255.255.255.255
    	       //Tests beyond minimum and maximum IP's - invalid
    		   "http://-1.-1.-1.-1", 
    		   "http://256.256.256.256"
       };
       
       for (String url : partition2)
    	   if(urlVal.isValid(url)) {
    		   System.out.println(url + " failed test as valid");
    	   } else {
    		   System.out.println(url + " is invalid as expected");
    	   }
   }
   
   //You may need to create more test cases for your Partitions   
   public void testYourThirdPartition()
   {
	   System.out.println("Partition Testing...");
       UrlValidator urlVal = new UrlValidator();
       
       //Tests no ports or IP address used
       if (urlVal.isValid("http://www.google.com")) {
    	   System.out.println("http://www.google.com is valid as expected");
       } else {
    	   System.out.println("http://www.google.com failed test as invalid");
       }
  }  
   
   
   public void testIsValid()
   {
	   //According to the documentation: 
	   // If schemes[] was initialized to a non null,
	   // then only those schemes are allowed.
	   // Otherwise the default schemes are "http", "https", "ftp".
	   // Matching is case-blind
	   
	   //From Wikipedia:
	   //Scheme component should be followed by a colon (:), consisting of a sequence 
	   // of characters beginning with a letter and followed by any combination of letters,
	   // digits, plus (+), period (.), or hyphen (-). It should should be
	   // ended by two slashes "//"
	   
	   System.out.println("Unit Testing...");
       UrlValidator urlVal = new UrlValidator();

	   String validSchemes[] = {
			   //all defaulted as valid
			   "https://",
			   "http://",
			   "ftp://",
	
			   //changing the case - should be case-blind
			   "HTTP://"
	   };
	   
	   String invalidSchemes[] = {
			   //changing the ending of the scheme to an invalid ending
			   "http",
			   "http:/",
			   "http//",
			   "http/",
			   "htt://",
			   
			   //no scheme should default to invalid
			   ""
	   };
	   
	   //
	   String validAuthority[] = {
			   //testing different domain extensions 
			   "www.oregonstate.edu",
			   "www.usa.gov",
			   "www.asp.net",
			   
			   //testing authorities with lack of "www"
			   "oregonstate.edu",
			   "1.2.3.4"		   
	   };
	   
	   String invalidAuthority[] = {
			   //no authority
			   "",
			   
			   //no domain name
			   ".abc",
			   "com",
			   
			   //incorrect dot-decimal notation
			   "1.2.3.4.5",
			   "1.2.3",
			   "a.b.c.d",
			   
			   //invalid domain extensions
			   "www.google.c0m",
			   "abc.123",

	   };
	   
	   String validPort[] = {
			   //ports not required
			   "",
			   
			   //ports must fall in range
			   ":80",
			   ":1",
			   ":65535",
	   };
	   
	   String invalidPort[] = {
			   //ports must only contain numbers
			   ":abc",
			   ":1a",
			   			   
			   //ports must fall in range
			   ":-1"			  
	   };
	   
	   String validPath[] = {
			   //URLs do not require a path
			   "",
			   
			   //Valid characters in path:  a-z A-Z 0-9 . - _ ~ ! $ & ' ( ) * + , ; = : @
			   //https://stackoverflow.com/questions/4669692/valid-characters-for-directory-part-of-a-url-for-short-links?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
			   //testing numbers and letters		   
			   "/path",
			   "/PATH",
			   "/0123",
			   
			   //testing combinations
			   "/path123",
			   "/123path",
			   
			   //testing multiple slashes
			   "/path/123",
			   "/path/123/path2",
			   
			   //testing other characters
			   "/*path",
			   "/$path",
			   "/.path",
			   "/+path",
			   "/=path",
			   "/@path",
			   "/:path",
			   "/&path",
			   "/(path",
			   "/)path",
			   "/;path",
			   "/'path"
			  
	   };
	   
	   String invalidPath[] = {
			   //path segments must be separated by a single forward slash '/'
			   "badpath/",
			   "badpath//",
			   "badpath",
			   "\\badpath"
	   };
	   
	   String validQuery[] = {
			   //queries are not required
			   "",
			   
			   //query with single parameter
			   "?articleid=232",
			   
			   //query with multiple parameters
			   "?articleid=232&itemid=abc"
			   
	   };
	   
	   String invalidQuery[] = {
			   //improper formatting
			   "??articleid=232",
			   "&articleid=232"
	   };
   
   //SHOULD RETURN VALID
	   System.out.println("Valid url Testing...");
	   int allValidFailed = 0;
	   int allValidTotal = 0;
   // All Combinations Of:
   // valid scheme + valid authority + valid port + valid path + valid query
	   for (int i = 0; i < validSchemes.length; i++) {
		   for(int j = 0; j < validAuthority.length; j++) {
			   for (int k = 0; k < validPort.length; k++) {
				   for (int n = 0; n < validPath.length; n++) {
					   for (int m = 0; m < validQuery.length; m++) {
						   String testURL = validSchemes[i] + validAuthority[j] + 
								   	validPort[k] + validPath[n] + validQuery[m];
						   allValidTotal++;

						   if(!urlVal.isValid(testURL)) {
							   allValidFailed++;
				    		   //System.out.println(testURL + " failed test as invalid.");
				    	   }
					   }
				   }
			   }
		   }
	   }
	   System.out.println("Number of valid urls marked invalid: " + allValidFailed + 
			   " out of " + allValidTotal);
   

   //SHOULD RETURN INVALID
	   System.out.println("Invalid Scheme Testing...");
  int invalidSchemeFailed = 0;
  int invalidSchemeTotal = 0;
   // Combinations Of:
   //invalid scheme + valid authority + valid port + valid path + valid query
   for (int i = 0; i < invalidSchemes.length; i++) {
					   String testURL = invalidSchemes[i] + validAuthority[0] + 
							   	validPort[0] + validPath[0] + validQuery[0];
					   invalidSchemeTotal++;				   
					   if(urlVal.isValid(testURL)) {
						   invalidSchemeFailed++;
			    		   System.out.println(testURL + " failed test as valid.");
			    	   }
   }
   System.out.println("Number of urls with invalid schemes marked valid: " + 
		   invalidSchemeFailed + " out of " + invalidSchemeTotal);
   
   
   int invalidAuthFailed = 0;
   int invalidAuthTotal = 0;
   // Combinations Of:
   //valid scheme + invalid authority + valid port + valid path + valid query
   System.out.println("Invalid Authority Testing...");
   for(int j = 0; j < invalidAuthority.length; j++) {		  
	   String testURL = validSchemes[0] + invalidAuthority[j] + 
			   	validPort[0] + validPath[0] + validQuery[0];
	   invalidAuthTotal++;
	   
	   if(urlVal.isValid(testURL)) {
		   invalidAuthFailed++;
		   System.out.println(testURL + " failed test as valid.");
	   }
   }
   System.out.println("Number of urls with invalid authorities marked valid: " + 
		   invalidAuthFailed + " out of " + invalidAuthTotal);
   
   int invalidPortFailed = 0;
   int invalidPortTotal = 0;

   // Combinations Of:
   //valid scheme + valid authority + invalid port + valid path + valid query
   System.out.println("Invalid Port Testing...");
   for (int k = 0; k < invalidPort.length; k++) {
	   String testURL = validSchemes[0] + validAuthority[0] + 
			   	invalidPort[k] + validPath[0] + validQuery[0];
	   invalidPortTotal++;	   
	   if(urlVal.isValid(testURL)) {
		   invalidPortFailed++;
		   System.out.println(testURL + " failed test as valid.");
	   }			  
   }
   System.out.println("Number of urls with invalid ports marked valid: " + 
		   invalidPortFailed + " out of " + invalidPortTotal);
   
   int invalidPathFailed = 0;
   int invalidPathTotal = 0;
   System.out.println("Invalid Path Testing...");
   // Combinations Of:
   //valid scheme + valid authority + valid port + invalid path + valid query  
	for (int n = 0; n < invalidPath.length; n++) {
				   
	   String testURL = validSchemes[0] + validAuthority[0] + 
			   	validPort[0] + invalidPath[n] + validQuery[0];
	   invalidPathTotal++;
	   
	   if(urlVal.isValid(testURL)) {
		   invalidPathFailed++;
		   System.out.println(testURL + " failed test as valid.");
	   }	
	}
	System.out.println("Number of urls with invalid paths marked valid: " + 
			   invalidPathFailed + " out of " + invalidPathTotal);
	
   int invalidQueryFailed = 0;
   int invalidQueryTotal = 0;

   // All Combinations Of:
   //valid scheme + valid authority + valid port + valid path + invalid query
   System.out.println("Invalid Query Testing...");
   for (int m = 0; m < invalidQuery.length; m++) {
	   String testURL = validSchemes[0] + validAuthority[0] + 
			   	validPort[0] + validPath[0] + invalidQuery[m];
	   invalidQueryTotal++;					   
	   if(urlVal.isValid(testURL)) {
		   invalidQueryFailed++;
		   System.out.println(testURL + " failed test as valid.");
	   }
   }
   System.out.println("Number of urls with invalid queries marked valid: " + 
		   invalidQueryFailed + " out of " + invalidQueryTotal);  
 }
   
}

 
