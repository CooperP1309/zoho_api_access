# Zoho API Access

### Usage

#### Usage Prerequisites
Using this requires W11/W10 with MSYS2 MINGW installed! (See 'extra notes' for exact dependencies)

To use APIs in Zoho, you must create a 'Self Client' in the zoho api console here: https://api-console.zoho.com.au/

- Select "Add Client" -> Select "Self Client" and use the Client ID and Client Secret in the following steps.

#### Steps

1. Clone this directory
```
git clone https://github.com/CooperP1309/zoho_api_access
```

2. Change into the root of the repo and make a 'client_credentials.txt'
```
cd zoho_api_access
touch client_credentials.txt
```

3. Populate the txt file with the following fields from your 'Self Client' in Zoho api   
```
client_id= '1000.M5435FGDTHDTJTJD2SAFAWWG'
client_secret= 'a978fdgsdg97j89dfb9f8tj7d987bc7dh9etj7'
grant_type= 'client_credentials'
scope= 'ZohoCRM.contacts.READ'
host= 'https://accounts.zoho.com.au'
zsoid= 'ZohoCRM.YOUR COMPANY'
```   
NOTE: Use this example of how to figure out your zsoid:   
```https://www.zoho.com/crm/developer/docs/api/v8/client-credentials.html```   
NOTE: The layout of your file must match the above example. Including use of apostrophes.  

4. Run app.exe
On success, your api access token will be printed. The source code has been left as to allow for a base to build off.  

### Extra Notes
- This is an implementation for Windows devices as that's what most IT companies use (as far as I'm aware) 
- I built this using MSYS2 MINGW (Install here: https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2)
- Developing with this repo requires libcurl (Easy install: https://everything.curl.dev/install/windows/win-msys2.html)
