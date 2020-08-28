<template>
  <div id="History-Statistics">
    <div class="history-select">
      <i-button class="realtime-data" type="warning"
                @click="showRealtimeData">实时数据</i-button>

      <!-- 历史查询, 日期选择器 -->
      <div class="history-date">
        <Date-picker class="date-picker" type="date" placeholder="选择日期"
                     format="yyyy-MM-dd" :value="historyDate" 
                     @on-change="historyDate=arguments[0]"></Date-picker>
        <i-button class="query-btn" type="warning"
                  @click="showHistoryData">查询历史</i-button>
      </div>
      <!-- 统计信息, 温度/湿度/光照 统计数据 -->
      <div class="info-statistics">
        <div class="abnormal-statistics">温度异常：{{statisticInfo.abnormalTemperature}}次</div>
        <div class="min-statistics">最低：{{statisticInfo.minTemperature}}℃</div>
        <div class="max-statistics">最高：{{statisticInfo.maxTemperature}}℃</div>
      </div>
      <div class="info-statistics">
        <div class="abnormal-statistics">湿度异常：{{statisticInfo.abnormalHumidity}}次</div>
        <div class="min-statistics">最低：{{statisticInfo.minHumidity}}％</div>
        <div class="max-statistics">最高：{{statisticInfo.maxHumidity}}％</div>
      </div>
      <div class="info-statistics">
        <div class="abnormal-statistics">光照异常：{{statisticInfo.abnormalIlluminance}}次</div>
        <div class="min-statistics">最低：{{statisticInfo.minIlluminance}}％</div>
        <div class="max-statistics">最高：{{statisticInfo.maxIlluminance}}％</div>
      </div>
    </div>
    
    <!-- 温度/湿度/光照 数据走势图 -->
    <div class="data-trend">
      <ve-line class="tendency-chart" height="370px" 
               :settings="chartSettings"
               :colors="colors"
               :data="trendInfo"></ve-line>
    </div>

    <!-- 设备控制模块 -->
    <div class="device-control">
      <!-- 实时监控查看按钮 -->
      <i-button class="camera-control" type="success"
                @click="cameraModal=true">查看监控</i-button>
      <!-- 风扇设备控制 -->
      <div class="control-item">
        <div class="control-item-icon iconfont iconfengkong"></div>
        <Radio-group v-model="deviceStatus.fan" type="button"
                     @on-change="updateDeviceStatus">
          <Radio :label="0">关</Radio>
          <Radio :label="1">1档</Radio>
          <Radio :label="2">2档</Radio>
          <Radio :label="3">3档</Radio>
        </Radio-group>
      </div>
      <!-- 蜂鸣器设备控制 -->
      <div class="control-item">
        <div class="control-item-icon iconfont iconhorn"></div>
        <Radio-group v-model="deviceStatus.buzzer" type="button"
                     @on-change="updateDeviceStatus">
          <Radio :label="0">关</Radio>
          <Radio :label="1">开</Radio>
        </Radio-group>
      </div>
      <!-- 照明灯设备控制 -->
      <div class="control-item">
        <div class="control-item-icon iconfont iconzhaoming-kai"></div>
        <Radio-group v-model="deviceStatus.light" type="button"
                     @on-change="updateDeviceStatus">
          <Radio :label="0">关</Radio>
          <Radio :label="1">开</Radio>
        </Radio-group>
      </div>
    </div>
    <!-- 实时监控弹窗 -->
    <Modal title="实时监控" width="720px"
           class="camera-modal"
           v-model="cameraModal"
           :mask-closable="false">
      <div class="camera-box">
        <iframe class="camera-iframe" :src="cameraPath" frameborder="0">
        </iframe>
      </div>
    </Modal>
  </div>
</template>

<script>
import {mapState, mapGetters, mapMutations, mapActions} from "vuex";

export default {
  name: 'HistoryStatistics',
  data() {
    //  走势图表头设置
    this.chartSettings = {
      labelMap: {
        'time'       : '时间',
        'temperature': '温度',
        'humidity'   : '湿度',
        'illuminance': '照明',
      },
    }
    //  走势图颜色
    this.colors = ['#db6623','#5271C2', '#aa9e5c', '#d48265']
    return {
      //  实时监控弹窗状态
      cameraModal: false,
      //  历史查询日期
      historyDate: "",

      //  请求实时走势信息 定时器
      realtimeTrendTimer     : null,
      //  请求实时统计信息 定时器
      realtimeStatisticsTimer: null,
      //  更新设备状态 消抖定时器
      setDeviceStatusTimer: null,
    }
  },
  methods: {
    ...mapActions({
      reqRealtimeTrendData : "reqRealtimeTrendData",   //请求 实时走势数据
      reqRealtimeStatistics: "reqRealtimeStatistics",  //请求 实时统计数据
      reqHistoryData       : "reqHistoryData",         //请求 历史走势数据
      reqHistoryStatistics : "reqHistoryStatistics",   //请求 历史统计数据
      reqSetDeviceStatus   : "reqSetDeviceStatus"      //请求 更新设备状态
    }),
    ...mapMutations({
      setShowStatisticsMode: "setShowStatisticsMode"   //设置 显示模式(历史/实时)
    }),
    //  显示实时数据信息
    showRealtimeData() { this.setShowStatisticsMode(0); },
    //  查询/显示历史数据
    showHistoryData() {
      if (!this.historyDate) return;
      //  发送 日期请求历史数据/历史统计 
      this.reqHistoryData(this.historyDate);
      this.reqHistoryStatistics(this.historyDate);
      //  设置显示模式为历史模式
      this.setShowStatisticsMode(1);
    },
    //  发送 更新设备状态(风扇,喇叭,照明,..)
    updateDeviceStatus() { 
      //  定时器消抖
      if (this.setDeviceStatusTimer)
        clearTimeout(this.setDeviceStatusTimer);
      //  300毫秒消抖时长
      let timerLength = 300;  
      //  300毫秒后执行请求, 更新设备状态
      this.setDeviceStatusTimer = setTimeout(() => {
        this.reqSetDeviceStatus(); 
        this.setDeviceStatusTimer = null;
      }, timerLength);
    }
  },
  computed: {
    ...mapState({
      realtimeTemperature: "realtimeTemperature",  //实时温度参数值
      realtimeHumidity   : "realtimeHumidity",     //实时湿度参数值
      realtimeIlluminance: "realtimeIlluminance",  //实时光照参数值
      deviceStatus       : "deviceStatus",         //实时设备状态值
      showStatisticsMode : "showStatisticsMode",   //显示统计数据模式 0:实时 1:历史
      realtimeStatistics : "realtimeStatistics",   //实时统计数据值
      historyStatistics  : "historyStatistics",    //历史统计数据值
      realtimeTrendData  : "realtimeTrendData",    //实时走势数据值
      historyTrendData   : "historyTrendData"      //历史走势数据值
    }),
    //  根据显示状态 实时/历史 显示对应信息
    statisticInfo() {
      //  0:实时统计信息，1:历史统计信息
      return (this.showStatisticsMode==0) 
        ? this.realtimeStatistics 
        : this.historyStatistics;
    },
    //  走势图数据格式化
    trendInfo() {
      let chartData = { 
        columns: ["time", "temperature", "humidity", "illuminance"],
        rows: []
      };
      chartData.rows = (this.showStatisticsMode==0) ? this.realtimeTrendData : this.historyTrendData;
      return chartData;
    },
    //  实时监控网址
    cameraPath() {
      let { protocol, hostname } = document.location;
      return protocol + '//' + hostname + ':8080/?action=stream';
    }
  },
  mounted() {
    //  3秒循环请求
    let timerLength = 1000 * 3;
    //  开启 请求实时走势信息 定时器
    this.realtimeTrendTimer      = setInterval(this.reqRealtimeTrendData, timerLength);
    //  开启 请求实时异常信息统计 定时器
    this.realtimeStatisticsTimer = setInterval(this.reqRealtimeStatistics, timerLength);
  },
  beforeDestroy() {
    //  销毁 请求实时走势信息 定时器
    clearInterval(this.realtimeTrendTimer);
    //  销毁 请求实时异常信息统计 定时器
    clearInterval(this.realtimeStatisticsTimer);
  }
}
</script>

<style lang="scss"
       src="../style/history_statistics.scss">
</style>