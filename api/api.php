<?php

set_time_limit(30);

function req($link)
{
     $curl = curl_init($link);
        
    curl_setopt($curl, CURLOPT_USERAGENT, "KeyAuth");
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);

    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
    
    $response = curl_exec($curl);

    curl_close($curl);
    
    return $response;
}

function sha256($input)
{
    $hashed = hash('sha256', $input);
    return $hashed;
}

$updated_gamever = 0; 
$skey = "SELLERKEYHERE";

switch ($_POST['type']) 
{
        case 'inject':

                $ip = fetch_ip();
                if (check_black($ip))
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "blacklisted IP."
                    ))));
                }
            
                $gamever = $_POST['gamever'];
                if ($gamever < $updated_gamever || !isset($gamever))
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "outdated game version."
                    ))));
                }
        
                $userr = hex2bin($_POST['user']); 
                if (!isset($userr))
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "no username."
                    ))));
                }
        
                $verify_response = req("https://keyauth.win/api/seller/?sellerkey=$skey&type=verifyuser&user=$userr");
                $verify_json = json_decode($verify_response);
                 
                if (!$verify_json->success)
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "$userr"
                    ))));
                }
                
                $hwid = hex2bin($_POST['hwid']);
                if (!isset($hwid))
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "no hwid."
                    ))));
                }
                
                $user_data_response = req("https://keyauth.win/api/seller/?sellerkey=$skey&type=userdata&user=$userr");
                $user_data_json = json_decode($user_data_response);
                if (!$user_data_json->success)
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "unable to retrieve user data."
                    ))));
                }
                
                if (time() > $user_data_json->subscriptions[0]->expiry)
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "expired subscription"
                    ))));   
                }
                
                $hwidd = $user_data_json->hwid;
                $hwidd = sha256($hwidd);
                if ($hwidd !== $hwid)
                {
                    die(bin2hex(json_encode(array(
                        "success" => false,
                        "message" => "invalid hwid."
                    ))));
                }
                
                $expiry = date('m/d/Y', $user_data_json->subscriptions[0]->expiry);
                
                die(bin2hex(json_encode(array(
                    "success" => true,
                    "message" => "successfully verified inject.",
                    "sub" => $user_data_json->subscriptions[0]->subscription,
                    "expiry" => $expiry
                ))));
}

?>