#include "include/cjparse.h"
#include "src/cjparse.cpp"
#include "src/cjparse_json_generate.cpp"
#include "src/cjparse_json_parser.cpp"
#include <string>

std::string json_5
    = R"({"explains":[],"count":15,"quotes":[{"exchange":"NYQ","shortname":"Agilent Technologies, Inc.","quoteType":"EQUITY","symbol":"A","index":"quotes","score":1.01035E7,"typeDisp":"Equity","longname":"Agilent Technologies, Inc.","exchDisp":"NYSE","sector":"Healthcare","sectorDisp":"Healthcare","industry":"Diagnostics & Research","industryDisp":"Diagnostics & Research","dispSecIndFlag":false,"isYahooFinance":true},{"exchange":"CMX","shortname":"Aluminum Futures,Feb-2026","quoteType":"FUTURE","symbol":"ALI=F","index":"quotes","score":3000300.0,"typeDisp":"Futures","exchDisp":"New York Commodity Exchange","isYahooFinance":true},{"exchange":"NGM","quoteType":"EQUITY","symbol":"AGRZ","index":"quotes","score":100007.0,"typeDisp":"Equity","longname":"Agroz Inc. Ordinary Shares","exchDisp":"NASDAQ","isYahooFinance":true},{"exchange":"NGM","quoteType":"EQUITY","symbol":"APACU","index":"quotes","score":100003.0,"typeDisp":"Equity","longname":"StoneBridge Acquisition II Corporation Units","exchDisp":"NASDAQ","sector":"Financial Services","sectorDisp":"Financial Services","industry":"Shell Companies","industryDisp":"Shell Companies","isYahooFinance":true},{"exchange":"NGM","shortname":"EMMIS ACQUISITION CORP. UNIT 1 ","quoteType":"EQUITY","symbol":"EMISU","index":"quotes","score":100001.0,"typeDisp":"Equity","longname":"Emmis Acquisition Corp.","exchDisp":"NASDAQ","sector":"Financial Services","sectorDisp":"Financial Services","industry":"Shell Companies","industryDisp":"Shell Companies","isYahooFinance":true},{"exchange":"NGM","shortname":"CSLM Digital Asset Acquisition ","quoteType":"EQUITY","symbol":"KOYNW","index":"quotes","score":100001.0,"typeDisp":"Equity","longname":"CSLM Digital Asset Acquisition Corp III, Ltd","exchDisp":"NASDAQ","sector":"Financial Services","sectorDisp":"Financial Services","industry":"Shell Companies","industryDisp":"Shell Companies","isYahooFinance":true},{"exchange":"CUL","shortname":"Securitized Art Index","symbol":"^CRWDARTS","score":0.0,"typeDisp":"Cultural Asset","longname":"Pricing Culture Securitized Art Index","isYahooFinance":true},{"exchange":"CUL","shortname":"Aston Martin Index","symbol":"^CLTBASTMA","score":0.0,"typeDisp":"Cultural Asset","longname":"Pricing Culture Aston Martin Index","isYahooFinance":true},{"exchange":"CUL","shortname":"Bored Ape Yacht Club Floor Index","symbol":"^CLTBBAYCL","score":0.0,"typeDisp":"Cultural Asset","longname":"Pricing Culture Bored Ape Yacht Club Floor Index","isYahooFinance":true}],"news":[{"uuid":"8467c30a-7484-36ae-a40f-5cc314868432","title":"Agilent Technologies, Inc. (A) Partners With AI Firm Lunit to Advance Cancer Diagnostics","publisher":"Insider Monkey","link":"https://finance.yahoo.com/news/agilent-technologies-inc-partners-ai-175644295.html","providerPublishTime":1759341404,"type":"STORY","thumbnail":{"resolutions":[{"url":"https://s.yimg.com/uu/api/res/1.2/qOIqanp6lJvohuuKI3833A--~B/aD04MTY7dz0xNDU2O2FwcGlkPXl0YWNoeW9u/https://media.zenfs.com/en/insidermonkey.com/090f63a10b52338b85aa052126f660e2","width":1456,"height":816,"tag":"original"},{"url":"https://s.yimg.com/uu/api/res/1.2/UDpxm85vV6qx8K5TUuqRFg--~B/Zmk9ZmlsbDtoPTE0MDtweW9mZj0wO3c9MTQwO2FwcGlkPXl0YWNoeW9u/https://media.zenfs.com/en/insidermonkey.com/090f63a10b52338b85aa052126f660e2","width":140,"height":140,"tag":"140x140"}]},"relatedTickers":["A","328130.KQ"]},{"uuid":"d2b5d8db-7f70-3396-a66f-f44f1c00acf7","title":"A: Raising target price to $135.00","publisher":"Argus Research","link":"https://finance.yahoo.com/m/d2b5d8db-7f70-3396-a66f-f44f1c00acf7/a%3A-raising-target-price-to.html","providerPublishTime":1759276800,"type":"STORY","thumbnail":{"resolutions":[{"url":"https://s.yimg.com/uu/api/res/1.2/SVKRhvYIgdoUv2HlogaP.A--~B/aD0wO3c9MDthcHBpZD15dGFjaHlvbg--/https://s.yimg.com/uc/fin/img/non-sub-report-thumb.png","width":0,"height":0,"tag":"original"}]},"relatedTickers":["A"]},{"uuid":"20c82ff0-69c5-35a8-8073-09d9e5942f3a","title":"Bruker, Agilent, Revvity, Avantor, and Bio-Techne Shares Are Soaring, What You Need To Know","publisher":"StockStory","link":"https://finance.yahoo.com/news/bruker-agilent-revvity-avantor-bio-200539969.html","providerPublishTime":1759262739,"type":"STORY","thumbnail":{"resolutions":[{"url":"https://s.yimg.com/uu/api/res/1.2/9EiHrdImxdXZ7seI484vRg--~B/aD03MDA7dz0xNDAwO2FwcGlkPXl0YWNoeW9u/https://media.zenfs.com/en/stockstory_922/6cee819fa1916b67603e477410fa6bda","width":1400,"height":700,"tag":"original"},{"url":"https://s.yimg.com/uu/api/res/1.2/q40oxUzNuHoUl3AxA0WGUw--~B/Zmk9ZmlsbDtoPTE0MDtweW9mZj0wO3c9MTQwO2FwcGlkPXl0YWNoeW9u/https://media.zenfs.com/en/stockstory_922/6cee819fa1916b67603e477410fa6bda","width":140,"height":140,"tag":"140x140"}]},"relatedTickers":["AVTR","BRKR","BRKRP","TECH","PFE","A","RVTY"]}],"nav":[],"lists":[],"researchReports":[{"reportHeadline":"Analyst Report: Agilent Technologies, Inc.","author":"Julie Utterback","reportDate":1756334216000,"id":"MS_0P0000007E_AnalystReport_1756334216000","provider":"Morningstar"},{"reportHeadline":"Morningstar | A Weekly Summary of Stock Ideas and Developments in the Companies We Cover","reportDate":1734741879000,"id":"MS_1258060_StockPicks_1734741879000","provider":"Morningstar"},{"reportHeadline":"Daily – Vickers Top Buyers & Sellers for 06/13/2024","reportDate":1718274448000,"id":"ARGUS_40288_TopBottomInsiderActivity_1718274448000","provider":"Argus"}],"screenerFieldResults":[],"totalTime":100,"timeTakenForQuotes":474,"timeTakenForNews":600,"timeTakenForAlgowatchlist":0,"timeTakenForPredefinedScreener":0,"timeTakenForCrunchbase":0,"timeTakenForNav":400,"timeTakenForResearchReports":800,"timeTakenForScreenerField":0,"timeTakenForCulturalAssets":750,"timeTakenForSearchLists":0})";

std::string json_4
    = R"({ "a": "value with double slash escape: \\\\\" still inside" })";

std::string json_3
    = R"({"model":"deepseek-r1:14b","created_at":"2025-02-05T20:05:12.569859Z","response":"\u003cthink\u003e \n the \n cool \n this will be raw \( \n pp ","done":false}
)";
std::string json_2 = R"({
        "Image": {
            "Width":  800,
            "Height": 600,
            "Title":  "View from 15th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/481989943",
                "Height": 125,
                "Width":  100
            },
            "Animated" : false,
            "IDs": [116, 943, 234, 38793]
          },
        "Image2": {
            "Width":  200,
            "Height": 900,
            "Title":  "View from 10th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/92837592",
                "Height": 260,
                "Width": 1000 
            },
            "Animated" : true,
            "IDs": [899, 831, 254, 648953]
          }
      })";

std::string json_1 = R"([
        {
           "precision": "zip",
           "Latitude":  37.7668,
           "Longitude": -122.3959,
           "Address":   "",
           "City":      "SAN FRANCISCO",
           "State":     "CA",
           "Zip":       "94107",
           "Country":   "US"
        }, 53,
        {
           "precision": "zip",
           "Latitude":  37.371991,
           "Longitude": -122.026020,
           "Address":   "",
           "City":      "SUNNYVALE",
           "State":     "CA",
           "Zip":       "94085",
           "Country":   "MX"
        },
        90,
  {"people": 93}
      ])";

int
main ()
{
  std::cout << "we are here" << '\n';
  cjparse parser (json_5);
  std::cout << "we are here 2" << '\n';
  cjparse_json_generator gen = cjparse_json_generator (parser.JSON, true);
  std::cout << "we are here 3" << '\n';
  std::cout << gen.JSON_string << '\n';
  return 0;
}
/*
int
main ()
{
    // construct the cjparse class inputing a JSON obliging string in.
    cjparse parser (json_2);

    // Print the JSON structure
    std::cout << "Parsed JSON: \n";
    cjparse_json_generator JSON_gen
        = cjparse_json_generator (parser.JSON, true);
    std::cout << JSON_gen.JSON_string << "\n";

    // testing return_the_value 1 input
    cjparse::json_value value_to_return = parser.return_the_value ("Image");
    JSON_gen = cjparse_json_generator (value_to_return, true);
    std::cout << "testing return the value in Image"
              << "\n"
              << JSON_gen.JSON_string << '\n';

    // testing return_the_value multiple inputs
    value_to_return
        = parser.return_the_value ({ "Image2", "Thumbnail", "Width" });
    JSON_gen = cjparse_json_generator (value_to_return, true);
    std::cout << "testing return the value in { \"Image2\", \"Thumbnail\", "
                 "\"Width\" }"
              << "\n"
              << JSON_gen.JSON_string << '\n';

    // testing check_if_type single name
    // checkiong if Width has a value of type "json_number"
    bool checking_if_number
        = parser.check_if_type<cjparse::json_object> ("Image");
    if (checking_if_number == true)
        {
            std::cout << "object named: " << "Image"
                      << "  has value type T (tempate)" << '\n';
        }
    else
        {
            std::cout << "object named: " << "Image"
                      << " has value type NOT T (template)" << '\n';
        }
    // testing check_if_type multiple names
    // checkiong if Width has a value of type "json_number"
    checking_if_number = parser.check_if_type<cjparse::json_number> (
        { "Image2", "Thumbnail", "Width" });
    if (checking_if_number == true)
        {
            std::cout << "object named: " << "Width"
                      << "  has value type T (tempate)" << '\n';
        }
    else
        {
            std::cout << "object named: " << "Width"
                      << " has value type NOT T (template)" << '\n';
        }

    return 0;
};
*/
