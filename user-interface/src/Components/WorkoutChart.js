import { Component } from "react";
import {
    LineChart,
    ResponsiveContainer,
    Legend, Tooltip,
    Line,
    XAxis,
    YAxis,
    CartesianGrid
} from 'recharts';
import results from "../generated_input.json"

function  getResultsData() {
    let displayData = [];

    results.forEach((element, index) => {
        displayData.push(
            {
                time: index,
                force: element
            }
        )
    })
    console.log(displayData);
    return(displayData);
}

export default class WorkoutChart extends Component{
    // time-force data
    // data = [
    //     {time: 1, force: 11,},
    //     {time: 2, force: 15,},
    //     {time: 3, force: 5,},
    //     {time: 4, force: 10,},
    //     {time: 5, force: 9,},
    //     {time: 6,force: 10,},
    // ];
    
    data = getResultsData();
    render(){
        return(
            <div>
                <ResponsiveContainer width="100%" aspect={3}>
                    <LineChart data={this.data} margin={{ right: 300 }}>
                        <CartesianGrid />
                            <XAxis dataKey="time (s)" interval={'preserveStartEnd'} />
                        <YAxis></YAxis>
                        <Legend />
                        <Tooltip />
                        <Line dataKey="force"
                            stroke="black" activeDot={{ r: 8 }} />
                    </LineChart>
                </ResponsiveContainer>
                
            </div> 
        );
    }
};