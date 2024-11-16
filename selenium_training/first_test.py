from selenium import webdriver
from selenium.webdriver.common.by import By #it is use to for locating elements
from selenium.webdriver.chrome.options import Options #for extensions
import time #pause the execution of the script for a certain amount of time

    # Indenting the lines inside the function
extension_path = r'C:/Users/gonzales.nel\Documents/browser_extension/SelectorsHub-XPath-Helper-Chrome-Web-Store.crx'
noclose_extend_options = Options()
noclose_extend_options.add_experimental_option("detach", True)  # Keeps the browser open after the script finishes

noclose_extend_options.add_extension(extension_path)

driver = webdriver.Chrome(options=noclose_extend_options)

driver.get("http://www.facebook.com")

time.sleep(5)
email = "test@gmail.com"
password = "password"
element = driver.find_element(By.ID, "email")
element.clear()
element.send_keys(email)

element_pass = driver.find_element(By.ID, "pass")
element_pass.clear()
element_pass.send_keys(password)

login_button_element = driver.find_element(By.NAME, "login")
login_button_element.click()
