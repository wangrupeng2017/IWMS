import axios from "axios"
// 响应拦截器
axios.interceptors.response.use(response => {        
  if (response.status === 200) {            
      return Promise.resolve(response.data);        
  } else {            
      return Promise.reject(response.data);        
  }    
}, error => {});

import Vue from "vue"
import Vuex from "vuex"
Vue.use(Vuex);

const state = {
  // 已选择仓库列表的索引
  chooseWarehouse: 0,
  // 仓库列表数据
  warehouseList: [
    {
      id: 1,
      name: "仓库1",
      router: "WarehouseNormal",
      image: "/static/house2.png"
    },
    {
      id: 2,
      name: "仓库2",
      router: "WarehouseMaintenance",
      image: "/static/house1.png"
    },
    {
      id: 3,
      name: "仓库3",
      router: "WarehouseMaintenance",
      image: "/static/house1.png"
    }
  ],
  // 仓库温度数据
  realtimeTemperature: {
    value:      28.3,  //当前值
    min:        20,  //最小值
    max:        30,  //最大值
    alarm:      0,   //监控报警设置 (0:关闭，1:开启)
    automation: 0,   //自动控制设置 (0:关闭，1:开启)
    status:     0,   //温度状态 (0:正常，1:异常 >0:异常)
  },
  // 仓库湿度数据
  realtimeHumidity: {
    value:      28.5,  //当前值
    min:        20,  //最小值
    max:        30,  //最大值
    alarm:      0,   //监控报警设置 (0:关闭，1:开启)
    automation: 0,   //自动控制设置 (0:关闭，1:开启)
    status:     0,   //温度状态 (0:正常，<0:异常 >0:异常)
  },
  // 仓库光照数据
  realtimeIlluminance: {
    value:      28,  //当前值
    min:        20,  //最小值
    max:        30,  //最大值
    alarm:      0,   //监控报警设置 (0:关闭，1:开启)
    automation: 0,   //自动控制设置 (0:关闭，1:开启)
    status:     0,   //温度状态 (0:正常，1:异常 >0:异常)
  },
  // 器件状态
  deviceStatus: {
    buzzer: 0,   //蜂鸣器状态 (0:关，1:开)
    light:  0,   //照明灯状态 (0:关，1:开)
    fan:    0,   //风扇状态   (0:关，1:1档，2:2档，3:3档)
  },

  // 统计信息显示模式
  // 0:显示实时统计信息，1:显示历史统计信息
  showStatisticsMode: 0,  
  // 实时异常信息统计
  realtimeStatistics: {
    abnormalTemperature: 0,
    minTemperature: 0,
    maxTemperature: 0,
    abnormalHumidity: 0,
    minHumidity: 0,
    maxHumidity: 0,
    abnormalIlluminance: 0,
    minIlluminance: 0,
    maxIlluminance: 0,
  },
  // 实时走势信息
  realtimeTrendData: [
    { time: "17:33", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:34", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:35", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:36", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:37", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:38", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:39", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:40", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
  ],
  // 历史异常信息统计
  historyStatistics: {
    abnormalTemperature: 0,
    minTemperature: 0,
    maxTemperature: 0,
    abnormalHumidity: 0,
    minHumidity: 0,
    maxHumidity: 0,
    abnormalIlluminance: 0,
    minIlluminance: 0,
    maxIlluminance: 0,
  },
  // 历史走势信息
  historyTrendData: [
    { time: "17:33", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:34", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:35", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:36", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:37", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:38", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:39", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
    { time: "17:40", temperature: 26.6, humidity: 20.0, illuminance: 70.5 },
  ],
};

const mutations = {
  // 更新选择仓库
  setChooseWarehouse(state, warehouseId) {
    state.chooseWarehouse = warehouseId;
  },
  // 更新统计信息显示模式(0实时/1历史)
  setShowStatisticsMode(state, mode) {
    state.showStatisticsMode = mode;
  },
  // 更新实时温度数据
  setRealtimeTemperature(state, data) {
    state.realtimeTemperature = data;
  },
  // 更新实时湿度数据
  setRealtimeHumidity(state, data) {
    state.realtimeHumidity = data;
  },
  // 更新实时光照数据
  setRealtimeIlluminance(state, data) {
    state.realtimeIlluminance = data;
  },
  // 更新设备状态信息
  setDeviceStatus(state, data) {
    state.deviceStatus = data;
  },
  // 更新实时走势数据
  setRealtimeTrendData(state, data) {
    if (state.realtimeTrendData.length > 60)
      state.realtimeTrendData.shift();
    state.realtimeTrendData.push(data);
  },
  // 更新实时统计数据
  setRealtimeStatistics(state, data) {
    state.realtimeStatistics = data;
  },
  // 更新历史走势数据
  setHistoryData(state, data) {
    state.historyTrendData = data;
  },
  // 更新历史统计信息
  setHistoryStatistics(state, data) {
    state.historyStatistics = data;
  }
};

const actions = {
  // 请求 实时温度数据
  reqRealtimeTemperature(context) {
    let {commit, state} = context;
    let warehouse = state.warehouseList[state.chooseWarehouse];
    let reqModel = {warehouse_id: warehouse.id};
    axios.post('/cgi-bin/realtime_temperature_param.cgi', reqModel).then(function(data) {
      console.log(data);
      commit('setRealtimeTemperature',data);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 实时湿度数据
  reqRealtimeHumidity(context) {
    let {commit, state} = context;
    let warehouse = state.warehouseList[state.chooseWarehouse];
    let reqModel = {warehouse_id: warehouse.id};
    axios.post('/cgi-bin/realtime_humidity_param.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setRealtimeHumidity',response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 实时照明数据
  reqRealtimeIlluminance(context) {
    let {commit, state} = context;
    let warehouse = state.warehouseList[state.chooseWarehouse];
    let reqModel = {warehouse_id: warehouse.id};
    axios.post('/cgi-bin/realtime_illumination_param.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setRealtimeIlluminance',response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 设备状态信息
  reqDeviceStatus(context) {
    let {commit, state} = context;
    let warehouse = state.warehouseList[state.chooseWarehouse];
    let reqModel = {warehouse_id: warehouse.id};

    axios.post('/cgi-bin/realtime_device_status.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setDeviceStatus',response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 更新仓库温度设置
  reqSetTemperatureConfig(context) {
    let {state} = context;
    let {chooseWarehouse, warehouseList, realtimeTemperature} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let {min, max, alarm, automation} = realtimeTemperature;
    let reqModel = { warehouse_id:warehouse.id, min, max, alarm, automation};

    axios.post('/cgi-bin/update_temperature_config.cgi', reqModel)
         .catch(function (error) { console.log(error); });
  },
  // 请求 更新仓库湿度设置
  reqSetHumidityConfig(context) {
    let {state} = context;
    let {chooseWarehouse, warehouseList, realtimeHumidity} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let {min, max, alarm, automation} = realtimeHumidity;
    let reqModel = { warehouse_id:warehouse.id, min, max, alarm, automation};

    axios.post('/cgi-bin/update_humidity_config.cgi', reqModel)
         .catch(function (error) { console.log(error); });
  },
  // 请求 更新仓库光照设置
  reqSetIlluminanceConfig(context) {
    let {state} = context;
    let {chooseWarehouse, warehouseList, realtimeIlluminance} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let {min, max, alarm, automation} = realtimeIlluminance;
    let reqModel = { warehouse_id:warehouse.id, min, max, alarm, automation};

    axios.post('/cgi-bin/update_illumination_config.cgi', reqModel)
         .catch(function (error) { console.log(error); });
  },
  // 请求 实时走势信息 
  reqRealtimeTrendData(context) {
    let {state, commit} = context;
    let {chooseWarehouse, warehouseList} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let reqModel = { warehouse_id:warehouse.id };

    axios.post('/cgi-bin/realtime_trend_data.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setRealtimeTrendData', response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 实时统计信息 
  reqRealtimeStatistics(context) {
    let {state, commit} = context;
    let {chooseWarehouse, warehouseList} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let reqModel = { warehouse_id:warehouse.id };

    axios.post('/cgi-bin/realtime_statistics_data.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setRealtimeStatistics',response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 历史走势数据
  reqHistoryData(context, date) {
    let {state, commit} = context;
    let {chooseWarehouse, warehouseList} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let reqModel = { warehouse_id:warehouse.id, date };
    
    axios.post('/cgi-bin/history_trend_data.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setHistoryData',response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 历史统计数据
  reqHistoryStatistics(context, date) {
    let {state, commit} = context;
    let {chooseWarehouse, warehouseList} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let reqModel = { warehouse_id:warehouse.id, date };

    axios.post('/cgi-bin/history_statistics_data.cgi', reqModel).then(function (response) {
      console.log(response);
      commit('setHistoryStatistics',response);
    }).catch(function (error) { console.log(error); });
  },
  // 请求 更新设备状态
  reqSetDeviceStatus(context) {
    let {state} = context;
    let {chooseWarehouse, warehouseList, deviceStatus} = state;
    let warehouse = warehouseList[chooseWarehouse];
    let {buzzer, light, fan} = deviceStatus;
    let reqModel = { warehouse_id:warehouse.id, buzzer, light, fan };
    
    axios.post('/cgi-bin/update_device_config.cgi', reqModel)
         .catch(function (error) { console.log(error); });
  }
};

const store = new Vuex.Store({
  state,
  mutations,
  actions,
});

export default store;
