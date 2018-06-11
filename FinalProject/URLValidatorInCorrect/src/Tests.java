

public class Tests {

	public static void main(String[] args) {
		UrlValidator urlValidator = new UrlValidator();
		if (urlValidator.isValid("www.google.com")) {
			 System.out.println("url is valid");
		} else {
			 System.out.println("url is invalid"); 
		}
	}

}
