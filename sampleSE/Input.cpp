#include"stdafx.h"
Input::Input()
{
}
//method to parse the input line by line and stores the required data in a map<ticker,Output Object>
//map<string,Output>& Input::ParseInput(const Reference &ref)
void Input::ParseInput(const Reference &ref, map<string, Output> &out)
{
		ifstream file(ref.input);
		string line;
		if (file.is_open())
		{
			//read file line by line
			while (getline(file, line))
			{
				vector<string> input_vec = split(line, ref.delimiter);
				
				Output out_obj;
				out_obj.diff_ask_bid = stod(input_vec[ref.ask - 1]) - stod(input_vec[ref.bid - 1]);
				//volume extracted from input file
				out_obj.volume = stod(input_vec[ref.volume - 1]);

				string::size_type sz;
				out_obj.timestamp = stol(input_vec[ref.timestamp - 1], &sz);


				map<string, Output>::iterator out_itr;
				//Search for the ticker if there is an entry already. If already present we update the
				//Output object with new values of min, max, max timestamp difference and formula
				out_itr = out.find(input_vec[ref.ticker - 1]);
				//for repetetive ticker
				if (out_itr != out.end())
				{
					//Output temp = out_itr->second;
					//Below if condition is to update the min(Ask-Bid) value
					if (out_itr->second.min > out_obj.diff_ask_bid)
						out_obj.min = out_obj.diff_ask_bid;
					else
						out_obj.min = out_itr->second.min;
					//Below if condition is to update the max(Ask-Bid) value
					if (out_itr->second.max < out_obj.diff_ask_bid)
						out_obj.max = out_obj.diff_ask_bid;
					else
						out_obj.max = out_itr->second.max;
					//Below if condition is to update the Maximum timestamp difference value
					if (out_itr->second.max_timestamp < (out_obj.timestamp - out_itr->second.timestamp))
						out_obj.max_timestamp = (out_obj.timestamp - out_itr->second.timestamp);
					else
						out_obj.max_timestamp = out_itr->second.max_timestamp;

					//update the sum of volumes for each timestamp
					out_obj.sum_volume = out_obj.volume + out_itr->second.sum_volume;
					
				}
				else
				{
					//for every new ticker
					out_obj.min = out_obj.diff_ask_bid;
					out_obj.max = out_obj.diff_ask_bid;
					out_obj.sum_volume = out_obj.volume;
					out_obj.max_timestamp = 0;
				}

				//function to find the values
				formula1(input_vec, out, ref, out_obj);

				out[input_vec[ref.ticker - 1]] = std::move(out_obj);
			}

		}
		file.close();
		//return out;
	
}
	