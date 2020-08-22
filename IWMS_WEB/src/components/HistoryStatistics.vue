<template>
  <div id="History-Statistics">
    <div class="history-select">
      <i-button class="realtime-data" type="warning"
                @click="showRealtimeData">实时数据</i-button>

      <!-- @todo 历史日期的绑定 -->
      <div class="history-date">
        <Date-picker class="date-picker" type="date" placeholder="选择日期"
                     v-model="historyDate"></Date-picker>
        <i-button class="query-btn" type="warning"
                  @click="showHistoryData">查询历史</i-button>
      </div>

      <div class="info-statistics">
        <div class="abnormal-statistics">温度异常：{{realtimeStatistics.abnormalTemperature}}次</div>
        <div class="min-statistics">最低：{{realtimeStatistics.minTemperature}}℃</div>
        <div class="max-statistics">最高：{{realtimeStatistics.maxTemperature}}℃</div>
      </div>
      <div class="info-statistics">
        <div class="abnormal-statistics">湿度异常：{{realtimeStatistics.abnormalHumidity}}次</div>
        <div class="min-statistics">最低：{{realtimeStatistics.minHumidity}}％</div>
        <div class="max-statistics">最高：{{realtimeStatistics.maxHumidity}}％</div>
      </div>
      <div class="info-statistics">
        <div class="abnormal-statistics">光照异常：{{realtimeStatistics.abnormalIlluminance}}次</div>
        <div class="min-statistics">最低：{{realtimeStatistics.minIlluminance}}％</div>
        <div class="max-statistics">最高：{{realtimeStatistics.maxIlluminance}}％</div>
      </div>
    </div>
    
    <!-- 走势图 -->
    <div class="data-trend">
      <ve-line class="tendency-chart" height="370px" 
               :settings="chartSettings"
               :data="trendInfo"></ve-line>
    </div>

    <div class="device-control">
      <i-button class="camera-control" type="success"
                @click="cameraModal=true">查看监控</i-button>

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
      <div class="control-item">
        <div class="control-item-icon iconfont iconhorn"></div>
        <Radio-group v-model="deviceStatus.buzzer" type="button"
                     @on-change="updateDeviceStatus">
          <Radio :label="0">关</Radio>
          <Radio :label="1">开</Radio>
        </Radio-group>
      </div>
      <div class="control-item">
        <div class="control-item-icon iconfont iconzhaoming-kai"></div>
        <Radio-group v-model="deviceStatus.light" type="button"
                     @on-change="updateDeviceStatus">
          <Radio :label="0">关</Radio>
          <Radio :label="1">开</Radio>
        </Radio-group>
      </div>
    </div>

    <Modal title="实时监控" width="800px"
           class="camera-modal"
           v-model="cameraModal"
           :mask-closable="false">
      <iframe class="camera-box" src="http://www.baidu.com" frameborder="0">
      </iframe>
    </Modal>
  </div>
</template>

<script>
import {mapState, mapGetters, mapMutations, mapActions} from "vuex";

export default {
  name: 'HistoryStatistics',
  data() {
    this.chartSettings = {
      labelMap: {
        'time'       : '时间',
        'temperature': '温度',
        'humidity'   : '湿度',
        'illuminance': '照明',
      },
    }
    return {
      // 摄像头对话框开关
      cameraModal: false,
      // 历史查询日期
      historyDate: "",

      // 请求实时走势信息 定时器
      realtimeTrendTimer     : null,
      // 请求实时统计信息 定时器
      realtimeStatisticsTimer: null,

      // 更新设备状态 消抖定时器
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
      setShowStatisticsMode: "setShowStatisticsMode"
    }),
    // 显示实时数据信息
    showRealtimeData() { this.setShowStatisticsMode(0); },
    // 查询/显示历史数据
    showHistoryData() {
      if (!this.historyDate) return;
      // 发送 日期请求历史数据/历史统计 然后 设置显示状态
      this.reqHistoryData(this.historyDate);
      this.reqHistoryStatistics(this.historyDate);
      this.setShowStatisticsMode(1);
    },
    // 发送 更新设备状态(风扇,喇叭,照明,..)
    updateDeviceStatus() { 
      if (this.setDeviceStatusTimer)
        clearTimeout(this.setDeviceStatusTimer);
      
      let timerLength = 300;  //300毫秒延时消抖
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
    // 根据显示状态 实时/历史 显示对应信息
    statisticInfo() {
      // 0:实时统计信息，1:历史统计信息
      return (this.showStatisticsMode==0) 
        ? this.realtimeStatistics 
        : this.historyStatistics;
    },
    // 走势图数据格式化
    trendInfo() {
      let chartData = { 
        columns: ["time", "temperature", "humidity", "illuminance"],
        rows: []
      };
      chartData.rows = (this.showStatisticsMode==0) ? this.realtimeTrendData : this.historyTrendData;
      return chartData;
    }
  },
  mounted() {
    // 3秒循环请求
    let timerLength = 1000 * 3;
    // 开启 请求实时走势信息 定时器
    this.realtimeTrendTimer      = setInterval(this.reqRealtimeTrendData, timerLength);
    // 开启 请求实时异常信息统计 定时器
    this.realtimeStatisticsTimer = setInterval(this.reqRealtimeStatistics, timerLength);
  },
  beforeDestroy() {
    // 销毁 请求实时走势信息 定时器
    clearInterval(this.realtimeTrendTimer);
    // 销毁 请求实时异常信息统计 定时器
    clearInterval(this.realtimeStatisticsTimer);
  }
}
</script>

<style lang="scss">
#History-Statistics {
  box-sizing: border-box;
  display: flex;
  justify-content: space-between;
  height: 400px;
  padding: 20px 100px;
  width: 100%;

  // 历史数据选择
  .history-select {
    background-color: #fff;
    box-sizing: border-box;
    box-shadow: 1px 1px 10px 2px #CCC;
    padding: 20px;
    height: 350px;
    width: 400px;

    // 实时数据按钮
    .realtime-data {
      font-size: 16px;
      height: 50px;
      width: 100%;
      & {
        background-color: #db6623;
        border-color: #db6623;
      }
      &:hover {
        background-color: mix(#fff,#db6623, 10%);
        border-color: mix(#fff,#db6623, 10%);
      }
      &:active {
        background-color: mix(#000,#db6623, 10%);
        border-color: mix(#000,#db6623, 10%);
      }
    }
    // 历史日期选择
    .history-date {
      display: flex;
      margin-top: 20px;
      margin-bottom: 10px;
      height: 40px;
      width: 100%;
      .date-picker {
        flex: 1;
        margin-right: 10px;
      }
      .query-btn {
        & {
          background-color: #db6623;
          border-color: #db6623;
        }
        &:hover {
          background-color: mix(#fff,#db6623, 10%);
          border-color: mix(#fff,#db6623, 10%);
        }
        &:active {
          background-color: mix(#000,#db6623, 10%);
          border-color: mix(#000,#db6623, 10%);
        }
      }
    }
    // 信息统计
    .info-statistics {
      display: flex;
      font-weight: 550;
      justify-content: space-between;
      margin-top: 25px;
    }
  }

  // 数据走势
  .data-trend {
    background-color: #fff;
    box-sizing: border-box;
    box-shadow: 1px 1px 10px 2px #CCC;
    padding: 20px 20px;
    height: 350px;
    width: calc((100% - 1500px) / 3 + 800px);
  }

  // 设备控制
  .device-control {
    background-color: #fff;
    box-sizing: border-box;
    box-shadow: 1px 1px 10px 2px #CCC;
    padding: 20px;
    height: 350px;
    width: 300px;

    .camera-control {
      font-size: 16px;
      height: 50px;
      width: 100%;
      & {
        background-color: #00cc66;
        border-color: #00cc66;
      }
      &:hover {
        background-color: mix(#fff,#00cc66, 10%);
        border-color: mix(#fff,#00cc66, 10%);
      }
      &:active {
        background-color: mix(#000,#00cc66, 10%);
        border-color: mix(#000,#00cc66, 10%);
      }
    }
    .control-item {
      align-items: center;
      display: flex;
      height: 40px;
      margin-top: 30px;
      width: 100%;
      .control-item-icon {
        font-size: 30px;
        margin-right: 20px;
      }
      &:nth-child(4) .control-item-icon {
        font-weight: 750;
      }
    }
  }
}

// 摄像头弹窗
.camera-modal {
  .camera-box {
    height: 500px;
    width: 100%;
  }
  .ivu-modal-footer .ivu-btn-text{
    display: none;
  }
}
</style>